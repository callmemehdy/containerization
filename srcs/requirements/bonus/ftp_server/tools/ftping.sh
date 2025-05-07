#!/bin/bash

mkdir -p /var/run/vsftpd/empty
chmod 755 /var/run/vsftpd/empty

adduser --disabled-password --gecos ",,," ftper

echo "ftper:$FTP_PASS" | chpasswd

mkdir -p /var/www/wordpress

# to make ftper get w access we use the full permissions num in group 
# chmod 775 /var/www/wordpress

# chown -R www-data:www-data /var/www/wordpress

usermod -d /var/www/wordpress ftper

usermod -aG www-data ftper

vsftpd /etc/vsftpd.conf
