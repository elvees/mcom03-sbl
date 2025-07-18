/*
 * Copyright (c) 2007-2017, Cameron Rich
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 * * Neither the name of the axTLS project nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file x509.c
 * 
 * Certificate processing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crypto_misc.h"
#include "config.h"

#ifdef CONFIG_SSL_CERT_VERIFICATION
static int x509_v3_subject_alt_name(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx);
static int x509_v3_basic_constraints(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx);
static int x509_v3_key_usage(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx);

/**
 * Retrieve the signature from a certificate.
 */
static const uint8_t *get_signature(const uint8_t *asn1_sig, int *len)
{
    int offset = 0;
    const uint8_t *ptr = NULL;

    if (asn1_next_obj(asn1_sig, &offset, ASN1_SEQUENCE) < 0 || 
            asn1_skip_obj(asn1_sig, &offset, ASN1_SEQUENCE))
        goto end_get_sig;

    if (asn1_sig[offset++] != ASN1_OCTET_STRING)
        goto end_get_sig;
    *len = get_asn1_length(asn1_sig, &offset);
    ptr = &asn1_sig[offset];          /* all ok */

end_get_sig:
    return ptr;
}

#endif

/**
 * Construct a new x509 object.
 * @return 0 if ok. < 0 if there was a problem.
 */
int x509_new(const uint8_t *cert, int *len, X509_CTX **ctx)
{
    int begin_tbs, end_tbs;
    int ret = X509_NOT_OK, offset = 0, cert_size = 0;
    int version = 0;
    X509_CTX *x509_ctx;
#ifdef CONFIG_SSL_CERT_VERIFICATION /* only care if doing verification */
    BI_CTX *bi_ctx;
#endif

    *ctx = (X509_CTX *)calloc(1, sizeof(X509_CTX));
    if (*ctx == NULL)
        return X509_MALLOC_ERROR;

    x509_ctx = *ctx;

    /* get the certificate size */
    asn1_skip_obj(cert, &cert_size, ASN1_SEQUENCE); 

    if (asn1_next_obj(cert, &offset, ASN1_SEQUENCE) < 0)
        goto end_cert;

    begin_tbs = offset;         /* start of the tbs */
    end_tbs = begin_tbs;        /* work out the end of the tbs */
    asn1_skip_obj(cert, &end_tbs, ASN1_SEQUENCE);

    if (asn1_next_obj(cert, &offset, ASN1_SEQUENCE) < 0)
        goto end_cert;

    /* optional version */
    if (cert[offset] == ASN1_EXPLICIT_TAG && 
            asn1_version(cert, &offset, &version) == X509_NOT_OK)
        goto end_cert;

    if (asn1_skip_obj(cert, &offset, ASN1_INTEGER) || /* serial number */ 
            asn1_next_obj(cert, &offset, ASN1_SEQUENCE) < 0)
        goto end_cert;

    /* make sure the signature is ok */
    if (asn1_signature_type(cert, &offset, x509_ctx))
    {
        ret = X509_VFY_ERROR_UNSUPPORTED_DIGEST;
        goto end_cert;
    }

    int asn1_name_ret = asn1_name(cert, &offset, x509_ctx->ca_cert_dn);
    if (asn1_name_ret != X509_OK)
    {
        ret = asn1_name_ret;
        goto end_cert;
    }

    if (asn1_validity(cert, &offset))
        goto end_cert;

    asn1_name_ret = asn1_name(cert, &offset, x509_ctx->cert_dn);
    if (asn1_name_ret != X509_OK)
    {
        ret = asn1_name_ret;
        goto end_cert;
    }

    int asn1_p_k_ret = asn1_public_key(cert, &offset, x509_ctx);
    if (asn1_p_k_ret != X509_OK) {
        ret = asn1_p_k_ret;
        goto end_cert;
    }

#ifdef CONFIG_SSL_CERT_VERIFICATION /* only care if doing verification */
    bi_ctx = x509_ctx->rsa_ctx->bi_ctx;

    /* use the appropriate signature algorithm */
    switch (x509_ctx->sig_type)
    {
        case SIG_TYPE_SHA256:
        {
            SHA256_CTX sha256_ctx;
            SHA256_Init(&sha256_ctx);
            SHA256_Update(&sha256_ctx, &cert[begin_tbs], end_tbs-begin_tbs);
            SHA256_Final(x509_ctx->sha256_digest, &sha256_ctx);
            x509_ctx->digest = bi_import(bi_ctx, x509_ctx->sha256_digest, SHA256_SIZE);
            if (x509_ctx->digest == NULL) {
                ret = X509_MALLOC_ERROR;
                goto end_cert;
            }
        }
            break;
    }

    if (version == 2 && asn1_next_obj(cert, &offset, ASN1_V3_DATA) > 0)
    {
        int alt_name_ret = x509_v3_subject_alt_name(cert, offset, x509_ctx);
        if (alt_name_ret != X509_OK) {
            ret = alt_name_ret;
            goto end_cert;
        }
        x509_v3_basic_constraints(cert, offset, x509_ctx);
        x509_v3_key_usage(cert, offset, x509_ctx);
    }

    offset = end_tbs;   /* skip the rest of v3 data */
    if (asn1_skip_obj(cert, &offset, ASN1_SEQUENCE))
        goto end_cert;
    int asn1_sig_ret = asn1_signature(cert, &offset, x509_ctx);
    if (asn1_sig_ret) {
        ret = asn1_sig_ret;
        goto end_cert;
    }

#endif
    ret = X509_OK;
end_cert:
    if (len)
    {
        *len = cert_size;
    }

    if (ret)
    {
#ifdef CONFIG_SSL_FULL_MODE
        printf("Error: Invalid X509 ASN.1 file (%s)\n",
                        x509_display_error(ret));
#endif
        x509_free(x509_ctx);
        *ctx = NULL;
    }

    return ret;
}

#ifdef CONFIG_SSL_CERT_VERIFICATION /* only care if doing verification */
static int x509_v3_subject_alt_name(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx)
{
    if ((offset = asn1_is_subject_alt_name(cert, offset)) > 0)
    {
        x509_ctx->subject_alt_name_present = true;
        x509_ctx->subject_alt_name_is_critical = 
                        asn1_is_critical_ext(cert, &offset);

        if (asn1_next_obj(cert, &offset, ASN1_OCTET_STRING) > 0)
        {
            int altlen;

            if ((altlen = asn1_next_obj(cert, &offset, ASN1_SEQUENCE)) > 0)
            {
                int endalt = offset + altlen;
                int totalnames = 0;

                while (offset < endalt)
                {
                    int type = cert[offset++];
                    int dnslen = get_asn1_length(cert, &offset);

                    if (type == ASN1_CONTEXT_DNSNAME)
                    {
                        x509_ctx->subject_alt_dnsnames = (char**)
                                realloc(x509_ctx->subject_alt_dnsnames, 
                                   (totalnames + 2) * sizeof(char*));
                        if (x509_ctx->subject_alt_dnsnames == NULL)
                            return X509_MALLOC_ERROR;

                        x509_ctx->subject_alt_dnsnames[totalnames] = 
                                (char*)malloc(dnslen + 1);
                        if (x509_ctx->subject_alt_dnsnames[totalnames] == NULL)
                            return X509_MALLOC_ERROR;

                        x509_ctx->subject_alt_dnsnames[totalnames+1] = NULL;
                        memcpy(x509_ctx->subject_alt_dnsnames[totalnames], 
                                cert + offset, dnslen);
                        x509_ctx->subject_alt_dnsnames[totalnames][dnslen] = 0;
                        totalnames++;
                    }

                    offset += dnslen;
                }
            }
        }
    }

    return X509_OK;
}

/**
 * Basic constraints - see https://tools.ietf.org/html/rfc5280#page-39
 */
static int x509_v3_basic_constraints(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx)
{
    int ret = X509_OK;
    int lenSeq = 0;

    if ((offset = asn1_is_basic_constraints(cert, offset)) == 0)
        goto end_contraints;

    x509_ctx->basic_constraint_present = true;
    x509_ctx->basic_constraint_is_critical = 
                    asn1_is_critical_ext(cert, &offset);

    /* Assign Defaults in case not specified
    basic_constraint_cA will already by zero by virtue of the calloc */
    x509_ctx->basic_constraint_cA = 0;
    /* basic_constraint_pathLenConstraint is unlimited by default. 
    10000 is just a large number (limits.h is not already included) */
    x509_ctx->basic_constraint_pathLenConstraint = 10000;
    
    if ((asn1_next_obj(cert, &offset, ASN1_OCTET_STRING) < 0) ||
            ((lenSeq = asn1_next_obj(cert, &offset, ASN1_SEQUENCE)) < 0))
    {
        ret = X509_NOT_OK;       
    }
    
    /* If the Sequence Length is greater than zero, 
    continue with the basic_constraint_cA */
    if ((lenSeq>0)&&(asn1_get_bool(cert, &offset, 
            &x509_ctx->basic_constraint_cA) < 0))
    {
        ret = X509_NOT_OK;
    }
    
    /* If the Sequence Length is greater than 3, it has more content than 
    the basic_constraint_cA bool, so grab the pathLenConstraint */
    if ((lenSeq>3) && (asn1_get_int(cert, &offset, 
            (int32_t *)&x509_ctx->basic_constraint_pathLenConstraint) < 0))
    {
        ret = X509_NOT_OK;
    }

end_contraints:
    return ret;
}

/*
 * Key usage - see https://tools.ietf.org/html/rfc5280#section-4.2.1.3
 */
static int x509_v3_key_usage(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx)
{
    int ret = X509_OK;

    if ((offset = asn1_is_key_usage(cert, offset)) == 0)
        goto end_key_usage;

    x509_ctx->key_usage_present = true;
    x509_ctx->key_usage_is_critical = asn1_is_critical_ext(cert, &offset);

    if (asn1_next_obj(cert, &offset, ASN1_OCTET_STRING) < 0 ||
            asn1_get_bit_string_as_int(cert, &offset, &x509_ctx->key_usage))
    {
        ret = X509_NOT_OK;       
    }

end_key_usage:
    return ret;
}
#endif

/**
 * Free an X.509 object's resources.
 */
void x509_free(X509_CTX *x509_ctx)
{
    X509_CTX *next;
    int i;

    if (x509_ctx == NULL)       /* if already null, then don't bother */
        return;

    for (i = 0; i < X509_NUM_DN_TYPES; i++)
    {
        free(x509_ctx->ca_cert_dn[i]);
        free(x509_ctx->cert_dn[i]);
    }

    free(x509_ctx->signature);

#ifdef CONFIG_SSL_CERT_VERIFICATION 
    if (x509_ctx->digest)
    {
        bi_free(x509_ctx->rsa_ctx->bi_ctx, x509_ctx->digest);
    }

    if (x509_ctx->subject_alt_dnsnames)
    {
        for (i = 0; x509_ctx->subject_alt_dnsnames[i]; ++i)
            free(x509_ctx->subject_alt_dnsnames[i]);

        free(x509_ctx->subject_alt_dnsnames);
    }
#endif

    RSA_free(x509_ctx->rsa_ctx);
    next = x509_ctx->next;
    if (next != NULL) {
        free(x509_ctx);
        x509_free(next);        /* clear the chain */
    }
}

#ifdef CONFIG_SSL_CERT_VERIFICATION
/**
 * Take a signature and decrypt it.
 */
static bigint *sig_verify(BI_CTX *ctx, const uint8_t *sig, int sig_len,
        bigint *modulus, bigint *pub_exp)
{
    int i, size;
    bigint *decrypted_bi, *dat_bi;
    bigint *bir = NULL;
    uint8_t *block = (uint8_t *)alloca(sig_len);

    /* decrypt */
    dat_bi = bi_import(ctx, sig, sig_len);
    ctx->mod_offset = BIGINT_M_OFFSET;

    /* convert to a normal block */
    decrypted_bi = bi_mod_power2(ctx, dat_bi, modulus, pub_exp);

    bi_export(ctx, decrypted_bi, block, sig_len);
    ctx->mod_offset = BIGINT_M_OFFSET;

    i = 10; /* start at the first possible non-padded byte */
    while (block[i++] && i < sig_len);
    size = sig_len - i;

    /* get only the bit we want */
    if (size > 0)
    {
        int len;
        const uint8_t *sig_ptr = get_signature(&block[i], &len);

        if (sig_ptr)
        {
            bir = bi_import(ctx, sig_ptr, len);
        }
    }

    /* save a few bytes of memory */
    bi_clear_cache(ctx);
    return bir;
}

/**
 * Do some basic checks on the certificate chain.
 *
 * Certificate verification consists of a number of checks:
 * - The date of the certificate is after the start date.
 * - The date of the certificate is before the finish date.
 * - A root certificate exists in the certificate store.
 * - That the certificate(s) are not self-signed.
 * - The certificate chain is valid.
 * - The signature of the certificate is valid.
 * - Basic constraints 
 */
int x509_verify(const CA_CERT_CTX *ca_cert_ctx, const X509_CTX *cert, 
        int *pathLenConstraint) 
{
    int ret = X509_OK, i = 0;
    bigint *cert_sig;
    X509_CTX *next_cert = NULL;
    BI_CTX *ctx = NULL;
    bigint *mod = NULL, *expn = NULL;
    int match_ca_cert = 0;
    uint8_t is_self_signed = 0;

    if (cert == NULL)
    {
        ret = X509_VFY_ERROR_NO_TRUSTED_CERT;       
        goto end_verify;
    }

    /* a self-signed certificate that is not in the CA store - use this 
       to check the signature */
    if (asn1_compare_dn(cert->ca_cert_dn, cert->cert_dn) == 0)
    {
        is_self_signed = 1;
        ctx = cert->rsa_ctx->bi_ctx;
        mod = cert->rsa_ctx->m;
        expn = cert->rsa_ctx->e;
    }

    if (cert->basic_constraint_present)
    {
        /* If the cA boolean is not asserted,
           then the keyCertSign bit in the key usage extension MUST NOT be
           asserted. */
        if (!cert->basic_constraint_cA &&
                IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_CERT_SIGN))
        {
            ret = X509_VFY_ERROR_BASIC_CONSTRAINT;
            goto end_verify;
        }

        /* The pathLenConstraint field is meaningful only if the cA boolean is
           asserted and the key usage extension, if present, asserts the
           keyCertSign bit.  In this case, it gives the maximum number of 
           non-self-issued intermediate certificates that may follow this 
           certificate in a valid certification path. */
        if (cert->basic_constraint_cA &&
            (!cert->key_usage_present || 
                IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_CERT_SIGN)) &&
            (cert->basic_constraint_pathLenConstraint+1) < *pathLenConstraint)
        {
            ret = X509_VFY_ERROR_BASIC_CONSTRAINT;
            goto end_verify;
        }
    }

    next_cert = cert->next;

    /* last cert in the chain - look for a trusted cert */
    if (next_cert == NULL)
    {
       if (ca_cert_ctx != NULL) 
       {
            /* go thru the CA store */
            while (i < CONFIG_X509_MAX_CA_CERTS && ca_cert_ctx->cert[i])
            {
                /* the extension is present but the cA boolean is not 
                   asserted, then the certified public key MUST NOT be used 
                   to verify certificate signatures. */
                if (cert->basic_constraint_present && 
                        !ca_cert_ctx->cert[i]->basic_constraint_cA)
                    continue;
                        
                if (asn1_compare_dn(cert->ca_cert_dn,
                                            ca_cert_ctx->cert[i]->cert_dn) == 0)
                {
                    /* use this CA certificate for signature verification */
                    match_ca_cert = true;
                    ctx = ca_cert_ctx->cert[i]->rsa_ctx->bi_ctx;
                    mod = ca_cert_ctx->cert[i]->rsa_ctx->m;
                    expn = ca_cert_ctx->cert[i]->rsa_ctx->e;


                    break;
                }

                i++;
            }
        }

        /* couldn't find a trusted cert (& let self-signed errors 
           be returned) */
        if (!match_ca_cert && !is_self_signed)
        {
            ret = X509_VFY_ERROR_NO_TRUSTED_CERT;       
            goto end_verify;
        }
    }
    else if (asn1_compare_dn(cert->ca_cert_dn, next_cert->cert_dn) != 0)
    {
        /* check the chain */
        ret = X509_VFY_ERROR_INVALID_CHAIN;
        goto end_verify;
    }
    else /* use the next certificate in the chain for signature verify */
    {
        ctx = next_cert->rsa_ctx->bi_ctx;
        mod = next_cert->rsa_ctx->m;
        expn = next_cert->rsa_ctx->e;
    }

    /* cert is self signed */
    if (!match_ca_cert && is_self_signed)
    {
        ret = X509_VFY_ERROR_SELF_SIGNED;
        goto end_verify;
    }

    /* check the signature */
    bigint *mod_clone = bi_clone(ctx, mod);
    if (mod_clone == NULL) {
        return X509_MALLOC_ERROR;
    }
    bigint *expn_clone = bi_clone(ctx, expn);
    if (expn_clone == NULL) {
        return X509_MALLOC_ERROR;
    }

    cert_sig = sig_verify(ctx, cert->signature, cert->sig_len, 
                        mod_clone, expn_clone);

    if (cert_sig && cert->digest)
    {
        if (bi_compare(cert_sig, cert->digest) != 0)
            ret = X509_VFY_ERROR_BAD_SIGNATURE;


        bi_free(ctx, cert_sig);
    }
    else
    {
        ret = X509_VFY_ERROR_BAD_SIGNATURE;
    }

    if (ret)
        goto end_verify;

    /* go down the certificate chain using recursion. */
    if (next_cert != NULL)
    {
        (*pathLenConstraint)++; /* don't include last certificate */
        ret = x509_verify(ca_cert_ctx, next_cert, pathLenConstraint);
    }

end_verify:
    return ret;
}
#endif

#if defined (CONFIG_SSL_FULL_MODE)
/**
 * Used for diagnostics.
 */
static const char *not_part_of_cert = "<Not Part Of Certificate>";
void x509_print(const X509_CTX *cert, CA_CERT_CTX *ca_cert_ctx) 
{
    if (cert == NULL)
        return;

    printf("=== CERTIFICATE ISSUED TO ===\n");
    printf("Common Name (CN):\t\t");
    printf("%s\n", cert->cert_dn[X509_COMMON_NAME] ?
                    cert->cert_dn[X509_COMMON_NAME] : not_part_of_cert);

    printf("Organization (O):\t\t");
    printf("%s\n", cert->cert_dn[X509_ORGANIZATION] ?
        cert->cert_dn[X509_ORGANIZATION] : not_part_of_cert);

    if (cert->cert_dn[X509_ORGANIZATIONAL_UNIT]) 
    {
        printf("Organizational Unit (OU):\t");
        printf("%s\n", cert->cert_dn[X509_ORGANIZATIONAL_UNIT]);
    }

    if (cert->cert_dn[X509_LOCATION]) 
    {
        printf("Location (L):\t\t\t");
        printf("%s\n", cert->cert_dn[X509_LOCATION]);
    }

    if (cert->cert_dn[X509_COUNTRY]) 
    {
        printf("Country (C):\t\t\t");
        printf("%s\n", cert->cert_dn[X509_COUNTRY]);
    }

    if (cert->cert_dn[X509_STATE]) 
    {
        printf("State (ST):\t\t\t");
        printf("%s\n", cert->cert_dn[X509_STATE]);
    }

    if (cert->basic_constraint_present)
    {
        printf("Basic Constraints:\t\t%sCA:%s, pathlen:%d\n",
                cert->basic_constraint_is_critical ? 
                    "critical, " : "",
                cert->basic_constraint_cA? "TRUE" : "FALSE",
                cert->basic_constraint_pathLenConstraint);
    }

    if (cert->key_usage_present)
    {
        printf("Key Usage:\t\t\t%s", cert->key_usage_is_critical ? 
                    "critical, " : "");
        bool has_started = false;

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_DIGITAL_SIGNATURE))
        {
            printf("Digital Signature");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_NON_REPUDIATION))
        {
            if (has_started)
                printf(", ");

            printf("Non Repudiation");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_ENCIPHERMENT))
        {
            if (has_started)
                printf(", ");

            printf("Key Encipherment");
            has_started = true;
        }
        
        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_DATA_ENCIPHERMENT))
        {
            if (has_started)
                printf(", ");

            printf("Data Encipherment");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_AGREEMENT))
        {
            if (has_started)
                printf(", ");

            printf("Key Agreement");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_CERT_SIGN))
        {
            if (has_started)
                printf(", ");

            printf("Key Cert Sign");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_CRL_SIGN))
        {
            if (has_started)
                printf(", ");

            printf("CRL Sign");
            has_started = true;
        }
       
        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_ENCIPHER_ONLY))
        {
            if (has_started)
                printf(", ");

            printf("Encipher Only");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_DECIPHER_ONLY))
        {
            if (has_started)
                printf(", ");

            printf("Decipher Only");
            has_started = true;
        }

        printf("\n");
    }

    if (cert->subject_alt_name_present)
    {
        printf("Subject Alt Name:\t\t%s", cert->subject_alt_name_is_critical 
                ?  "critical, " : "");
        if (cert->subject_alt_dnsnames)
        {
            int i = 0;

            while (cert->subject_alt_dnsnames[i])
                printf("%s ", cert->subject_alt_dnsnames[i++]);
        }
        printf("\n");

    }

    printf("=== CERTIFICATE ISSUED BY ===\n");
    printf("Common Name (CN):\t\t");
    printf("%s\n", cert->ca_cert_dn[X509_COMMON_NAME] ?
                    cert->ca_cert_dn[X509_COMMON_NAME] : not_part_of_cert);

    printf("Organization (O):\t\t");
    printf("%s\n", cert->ca_cert_dn[X509_ORGANIZATION] ?
        cert->ca_cert_dn[X509_ORGANIZATION] : not_part_of_cert);

    if (cert->ca_cert_dn[X509_ORGANIZATIONAL_UNIT]) 
    {
        printf("Organizational Unit (OU):\t");
        printf("%s\n", cert->ca_cert_dn[X509_ORGANIZATIONAL_UNIT]);
    }

    if (cert->ca_cert_dn[X509_LOCATION]) 
    {
        printf("Location (L):\t\t\t");
        printf("%s\n", cert->ca_cert_dn[X509_LOCATION]);
    }

    if (cert->ca_cert_dn[X509_COUNTRY]) 
    {
        printf("Country (C):\t\t\t");
        printf("%s\n", cert->ca_cert_dn[X509_COUNTRY]);
    }

    if (cert->ca_cert_dn[X509_STATE]) 
    {
        printf("State (ST):\t\t\t");
        printf("%s\n", cert->ca_cert_dn[X509_STATE]);
    }

    printf("Not Before:\t\t\t%s", ctime(&cert->not_before));
    printf("Not After:\t\t\t%s", ctime(&cert->not_after));
    printf("RSA bitsize:\t\t\t%d\n", cert->rsa_ctx->num_octets*8);
    printf("Sig Type:\t\t\t");
    switch (cert->sig_type)
    {
        case SIG_TYPE_MD5:
            printf("MD5\n");
            break;
        case SIG_TYPE_SHA1:
            printf("SHA1\n");
            break;
        case SIG_TYPE_SHA256:
            printf("SHA256\n");
            break;
        case SIG_TYPE_SHA384:
            printf("SHA384\n");
            break;
        case SIG_TYPE_SHA512:
            printf("SHA512\n");
            break;
        default:
            printf("Unrecognized: %d\n", cert->sig_type);
            break;
    }

    if (ca_cert_ctx)
    {
        int pathLenConstraint = 0;
        printf("Verify:\t\t\t\t%s\n",
                x509_display_error(x509_verify(ca_cert_ctx, cert,
                        &pathLenConstraint)));
    }

#if 0
    print_blob("Signature", cert->signature, cert->sig_len);
    bi_print("Modulus", cert->rsa_ctx->m);
    bi_print("Pub Exp", cert->rsa_ctx->e);
#endif

    if (ca_cert_ctx)
    {
        x509_print(cert->next, ca_cert_ctx);
    }

    TTY_FLUSH();
}

const char * x509_display_error(int error)
{
    switch (error)
    {
        case X509_OK:
            return "Certificate verify successful";

        case X509_NOT_OK:
            return "X509 not ok";

        case X509_VFY_ERROR_NO_TRUSTED_CERT:
            return "No trusted cert is available";

        case X509_VFY_ERROR_BAD_SIGNATURE:
            return "Bad signature";

        case X509_VFY_ERROR_NOT_YET_VALID:
            return "Cert is not yet valid";

        case X509_VFY_ERROR_EXPIRED:
            return "Cert has expired";

        case X509_VFY_ERROR_SELF_SIGNED:
            return "Cert is self-signed";

        case X509_VFY_ERROR_INVALID_CHAIN:
            return "Chain is invalid (check order of certs)";

        case X509_VFY_ERROR_UNSUPPORTED_DIGEST:
            return "Unsupported digest";

        case X509_INVALID_PRIV_KEY:
            return "Invalid private key";

        case X509_VFY_ERROR_BASIC_CONSTRAINT:
            return "Basic constraint invalid";

        default:
            return "Unknown";
    }
}
#endif      /* CONFIG_SSL_FULL_MODE */

