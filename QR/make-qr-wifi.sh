#!/bin/sh

QR=/usr/bin/qrencode
PREFIJO="Dom32io-"
MAC="${1}"

$QR -o "qr${MAC}.png" -l H -t PNG "WIFI:T:WPA;S:${PREFIJO}${MAC};P:;;"

