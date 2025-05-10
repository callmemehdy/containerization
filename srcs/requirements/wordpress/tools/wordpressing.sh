#!/bin/bash

# while ! mysqladmin ping -h"mariadb" -u"$MARIADB_USER" -p"$MARIADB_PASSWORD" --silent; do
#     sleep 1
# done


curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar

# apt install -y sendmail

chmod +x wp-cli.phar && mv wp-cli.phar /usr/local/bin/wp


# in case there multiuser group of www-data group
cd /var/www/wordpress

# if [ -f wp-config.php ] ; then
# 	echo "Wordpress is already installed brother!";
	
# fi


chown -R www-data:www-data  /var/www

chmod -R 775 /var/www/wordpress

#############
sleep 10

wp core download --allow-root

wp config create --dbhost=mariadb:3306 --dbname="$MARIADB_DB" --dbuser="$MARIADB_USER" --dbpass="$MARIADB_PASSWORD" --allow-root

wp config set WP_REDIS_HOST $REDIS_HOST --add --allow-root

wp config set WP_REDIS_PORT $REDIS_PORT --add --allow-root

wp plugin install redis-cache --activate --allow-root

# install the plugin

wp core install --url="$DOMAIN_NAME" --title="$WEBSITE_TITLE" --admin_user="$ADMIN_USER" \
				--admin_password="$ADMIN_PASS" --admin_email="$ADMIN_EMAIL" --allow-root

wp user create "$WP_USER" "$WP_USER_EMAIL" --role="$WP_USER_ROLE" --user_pass="$WP_USER_PASS" --allow-root

chown -R www-data:www-data  /var/www

chmod -R 775 /var/www/wordpress


wp redis enable --allow-root

wp db check --allow-root


/usr/sbin/php-fpm7.4 -F