FROM debian:bullseye

#unshare syscall

COPY ./tools/wordpressing.sh .

RUN apt update && apt upgrade -y && \
	apt install -y curl php7.4-fpm php-mysql\ 
	mariadb-client php7.4 redis php-redis


RUN mkdir -p /var/www/wordpress

RUN mkdir -p /run/php


RUN chmod +x wordpressing.sh

RUN sed -i 's#listen = /run/php/php7.4-fpm.sock#listen = 0.0.0.0:9000#' /etc/php/7.4/fpm/pool.d/www.conf


ENTRYPOINT ["./wordpressing.sh"]

