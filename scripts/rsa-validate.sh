#!/usr/bin/env bash

openssl rsa -in .cloak/private.pem -pubout -outform PEM | sha256sum
openssl rsa -in .cloak/public.pem -pubin -outform PEM | sha256sum
