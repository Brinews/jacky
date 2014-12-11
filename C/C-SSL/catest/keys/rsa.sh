#!/bin/sh

#gen rsa key
openssl genrsa -des3 -out $1.key 1024
#remove phase
#openssl rsa -in $1.key -out $1.key
#gen cert
openssl req -new -key $1.key -out $1.csr
