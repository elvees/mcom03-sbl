#!/usr/bin/env bash
# Copyright 2020-2021 RnD Center "ELVEES", JSC

set -euo pipefail

# 1. Generating a 3072-bit private RSA key of the root certificate in PEM format:
openssl genrsa -out rootPrivateKey.pem 3072
# The result can be viewed with the command: openssl pkey -text -noout -in rootPrivateKey.pem

# 2. Creating a self-signed root certificate:
openssl req -x509 -new -sha256 -key rootPrivateKey.pem -days 1024 -out rootCA.pem \
    -subj "/C=RU/ST=MSK/L=MSK/O=Elvees/OU=/CN=root/emailAddress=info@elvees.com"
# The result can be viewed with the command: openssl x509 -text -noout -in rootCA.pem

# 3. Creating a private RSA key of an intermediate certificate
# with a length of 3072 bits in PEM format:
openssl genrsa -out nonRootPrivateKey.pem 3072
# The result can be viewed with the command: openssl pkey -text -noout -in nonRootPrivateKey.pem

# 4. Creating a request to issue an intermediate certificate CSR (Certificate Signing Request),
# which will be signed with the private key of the intermediate certificate (step 3):
openssl req -new -key nonRootPrivateKey.pem -out nonRootCA.csr \
    -subj "/C=RU/ST=MSK/L=MSK/O=Elvees/OU=/CN=nonRoot/emailAddress=info@elvees.com"

# 5. To create an intermediate certificate v3 and sign it using the private key
# of the root certificate (step 1), it is needed to create a file with extensions
# (ca_intermediate.ext) with the following content:
#    [ v3_intermediate_ca ]
#    basicConstraints = CA:true

cat > ca_intermediate.ext <<EOF
[ v3_intermediate_ca ]
basicConstraints = CA:true
EOF

# and execute the following command:
openssl x509 -req -extfile ca_intermediate.ext -extensions v3_intermediate_ca \
    -in nonRootCA.csr -CA rootCA.pem -CAkey rootPrivateKey.pem \
    -CAcreateserial -out nonRootCA.pem -days 3650 -sha256

# 6. Generating the RSA private key of the end certificate
# with a length of 3072 bits in PEM format:
openssl genrsa -out fwPrivateKey.pem 3072

# 7. Creating a request to issue a end CSR certificate,
# which will be signed by the private key of the intermediate certificate (step 6):
openssl req -new -key fwPrivateKey.pem -out fwCA.csr \
    -subj "/C=RU/ST=MSK/L=MSK/O=Elvees/OU=/CN=fw/emailAddress=info@elvees.com"

# 8. To add the firmware version to the end certificate,
# it is needed to create a configuration file (payload.ext) with the following content:
#    [ payload_ext ]
#    basicConstraints = CA:false
#    1.6.5.4=ASN1:INTEGER:N

# where 1.6.5.4 - is the oid, corresponding to the risc0_fw_counter field
# N - is the firmware version counter.

cat > payload.ext <<EOF
[ payload_ext ]
basicConstraints = CA:false
1.6.5.4=ASN1:INTEGER:0
EOF

# This file must be used to generate the end certificate signed with the private key
# of the intermediate certificate (step 3):
openssl x509 -req -extfile payload.ext -extensions payload_ext -in fwCA.csr \
    -CA nonRootCA.pem -CAkey nonRootPrivateKey.pem -CAcreateserial \
    -out fwCA.pem -sha256 -days 3650
# It will be used to check the executable image

# 9. Converting private key of root certificate in PEM format to DER format:
openssl pkey -outform der -in rootPrivateKey.pem -out rootPrivateKey.der

# 10. Converting PEM root certificate to DER format:
openssl x509 -outform der -in rootCA.pem -out rootCA.der

# 11. Converting private key of intermediate certificate in PEM format to DER format:
openssl pkey -outform der -in nonRootPrivateKey.pem -out nonRootPrivateKey.der

# 12. Converting an intermediate certificate in PEM format to DER format:
openssl x509 -outform der -in nonRootCA.pem -out nonRootCA.der

# 13. Converting the private key of the end certificate in PEM format to DER format:
openssl pkey -outform der -in fwPrivateKey.pem -out fwPrivateKey.der

# 14. Converting an End Certificate in PEM Format to DER Format:
openssl x509 -outform der -in fwCA.pem -out fwCA.der

# 15. TAR certificates required for making FW:
tar -czvf certs.tar.gz rootCA.der nonRootCA.der fwCA.der fwPrivateKey.der
