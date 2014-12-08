#!/bin/sh

openssl genrsa -des3 -out CA.key 1024
openssl req -new -key CA.key -out CA.csr
openssl x509 -req -days 365 -in CA.csr -signkey CA.key -out CA.crt
