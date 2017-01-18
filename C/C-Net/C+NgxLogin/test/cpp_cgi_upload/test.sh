#!/bin/sh

ps -ef | grep up.cgi | grep -v grep | awk '{print $2}' | xargs kill -s 9
ps -ef | grep login.cgi | grep -v grep | awk '{print $2}' | xargs kill -s 9
ps -ef | grep update.cgi | grep -v grep | awk '{print $2}' | xargs kill -s 9

spawn-fcgi -a 192.168.112.11 -p 8081 -f /usr/share/nginx/html/up.cgi
spawn-fcgi -a 192.168.112.11 -p 8082 -f /usr/share/nginx/html/login.cgi
spawn-fcgi -a 192.168.112.11 -p 8083 -f /usr/share/nginx/html/update.cgi

curl -X POST -F 'username=test' -F 'password=111' http://192.168.112.11/login.cgi
curl -X POST -F 'username=test' -F 'password=111' -F 'nickname=ok' -F 'picname=/none/1.jpg' http://192.168.112.11/update.cgi
curl -X POST -F 'username=test' -F 'password=111' http://192.168.112.11/login.cgi
