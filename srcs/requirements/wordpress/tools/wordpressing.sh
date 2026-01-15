#!/bin/bash

if [ ! -f wp-config.php ]; then
    wp core download --allow-root

    wp config create \
        --dbhost=mariadb:3306 \
        --dbname="$MARIADB_DB" \
        --dbuser="$MARIADB_USER" \
        --dbpass="$MARIADB_PASSWORD" \
        --allow-root

    wp config set WP_REDIS_HOST "$REDIS_HOST" --add --allow-root
    wp config set WP_REDIS_PORT "$REDIS_PORT" --add --allow-root

    wp core install \
        --url="$DOMAIN_NAME" \
        --title="$WEBSITE_TITLE" \
        --admin_user="$ADMIN_USER" \
        --admin_password="$ADMIN_PASS" \
        --admin_email="$ADMIN_EMAIL" \
        --allow-root

    wp user create "$WP_USER" "$WP_USER_EMAIL" \
        --role="$WP_USER_ROLE" \
        --user_pass="$WP_USER_PASS" \
        --allow-root

    wp plugin install redis-cache --activate --allow-root
    wp redis enable --allow-root

    chown -R www-data:www-data /var/www/wordpress
fi

exec /usr/sbin/php-fpm8.2 -F