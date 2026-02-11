#!/bin/bash
# SPDX-License-Identifier: GPL-2.0+
# Copyright 2025 RnD Center "ELVEES", JSC

set -o errexit
set -o pipefail

function help {
    echo "Description:
    Utility is intended to get SHA256 of tbsCertificate of CERT in DER format.

Usage:
    $(basename "$0") [OPTION] CERT
Arguments:
    CERT        Path to certificate to extract tbsCertificate from and calculate SHA256
Options:
    -h          print this message and exit"
}

TBSCert=$(mktemp) || exit 1

trap 'rm -f "$TBSCert"' EXIT

while getopts "h" arg; do
    case "$arg" in
    h)
        help
        exit 0
    ;;
    *)
        echo "Incorrect option"
        help
        exit 1
    ;;
    esac
done

CERT=$1

if [ ! -f "$CERT" ]; then
    echo "The certificate doesn't exist"
    help
    exit 1
fi

SEQ=$(openssl asn1parse -inform der -in "$CERT" | sed -n 2p)
RE='^(.*):d=.*hl=(.*)l=(.*)cons:(.*)$'
if [[ ! "$SEQ" =~ $RE ]]; then
    echo "Failed to decode ASN.1 sequence"
    exit 1
fi

TYPE=$(echo "${BASH_REMATCH[4]}" | xargs)
if [[ "$TYPE" != "SEQUENCE" ]]; then
    echo "Incorrect certificate format"
    exit 1
fi

TBS_BEGIN=$(echo "${BASH_REMATCH[1]}" | xargs)
TBS_LEN=$(( BASH_REMATCH[2] + BASH_REMATCH[3] ))

dd if="$CERT" of="$TBSCert" bs=1 skip="$TBS_BEGIN" count="$TBS_LEN" >& /dev/null

echo -n $(sha256sum "$TBSCert" | cut -f 1 -d ' ')
