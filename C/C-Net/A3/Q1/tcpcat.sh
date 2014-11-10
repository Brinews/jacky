#!/bin/sh
sudo tcpdump -i eth1 host 172.16.0.4 -s 0 -w tcp_con_open_close.cap

