#!/bin/sh

#首先生成CA的key文件:
openssl genrsa -des3 -out ca.key 1024
#在生成CA自签名的证书:
openssl req -new -x509 -key ca.key -out ca.crt -days 365
