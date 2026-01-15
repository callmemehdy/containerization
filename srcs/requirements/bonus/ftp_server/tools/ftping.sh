#!/bin/bash

mkdir -p /var/run/vsftpd/empty
chmod 755 /var/run/vsftpd/empty

adduser --disabled-password --gecos ",,," ftper
echo "ftper:$FTP_PASS" | chpasswd

mkdir -p /var/www/wordpress
usermod -d /var/www/wordpress ftper
usermod -aG www-data ftper

exec vsftpd /etc/vsftpd.conf
