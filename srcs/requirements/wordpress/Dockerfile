FROM debian:bookworm

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y curl php8.2-fpm php-mysql php redis php-redis && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

RUN curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar && \
    chmod +x wp-cli.phar && \
    mv wp-cli.phar /usr/local/bin/wp

RUN mkdir -p /var/www/wordpress /run/php && \
    chown -R www-data:www-data /var/www && \
    chmod -R 775 /var/www/wordpress

RUN sed -i 's#listen = /run/php/php8.2-fpm.sock#listen = 0.0.0.0:9000#' /etc/php/8.2/fpm/pool.d/www.conf

WORKDIR /var/www/wordpress

COPY ./tools/wordpressing.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/wordpressing.sh

ENTRYPOINT ["wordpressing.sh"]
