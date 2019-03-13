#!/bin/bash
set -e
unset http_proxy
unset https_proxy
unset HTTP_PROXY
unset HTTPS_PROXY
unset ftp_proxy
unset FTP_PROXY
sudo route add default gw 192.168.8.1
sudo route del default gw 10.4.60.129
sudo route add -net 192.168.7.0/24 gw 10.4.60.129 eth0
sudo route add -net 192.168.2.0/24 gw 10.4.60.129 eth0
sudo route add -net 202.107.117.0/24 gw 10.4.60.129 eth0
