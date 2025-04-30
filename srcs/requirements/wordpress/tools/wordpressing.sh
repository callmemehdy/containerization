#!/bin/bash


curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar

apt install -y sendmail

chmod +x wp-cli.phar && mv wp-cli.phar /usr/local/bin/wp


# in case there multiuser group of www-data group
chown -R www-data:www-data  /var/www

chmod -R 755 /var/www/wordpress

cd /var/www/wordpress

wp core download --allow-root

wp core config	--dbhost=mariadb:3306 --dbname="$MARIADB_DB" --dbuser="$MARIADB_USER" \
				--dbpass="$MARIADB_PASSWORD" --allow-root

wp core install --url="$DOMAIN_NAME" --title="$WEBSITE_TITLE" --admin_user="$ADMIN_USER" \
				--admin_password="$ADMIN_PASS" --admin_email="$ADMIN_EMAIL" --allow-root #--locale=$LOCAL


wp db check --allow-root

/usr/sbin/php-fpm7.4 -F