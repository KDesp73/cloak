#!/usr/bin/env bash

PRIVATE_HASH=$(openssl rsa -in .cloak/private.pem -pubout -outform PEM | sha256sum)
PUBLIC_HASH=$(openssl rsa -in .cloak/public.pem -pubin -outform PEM | sha256sum)

if [ "$PRIVATE_HASH" == "$PUBLIC_HASH" ]; then
    echo "Valid"
else
    echo "Invalid"
fi
