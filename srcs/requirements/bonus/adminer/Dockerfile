FROM debian:bookworm

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y php curl php-mysql && \
    apt-get clean && rm -rf /var/lib/apt/lists/* && \
    mkdir -p /var/www/1337 && \
    chown -R www-data:www-data /var/www/1337

WORKDIR /var/www/1337

RUN curl -L -o index.php https://github.com/vrana/adminer/releases/download/v4.7.8/adminer-4.7.8.php && \
    chmod 775 index.php && \
    chown www-data:www-data index.php

ENTRYPOINT ["php", "-S", "0.0.0.0:9090", "-t", "/var/www/1337"]