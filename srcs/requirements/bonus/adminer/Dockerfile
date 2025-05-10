FROM	debian:bullseye
#DOCROOT
RUN		apt update && apt upgrade -y && apt install -y php curl php-mysql

RUN		mkdir -p /var/www/1337 && chown -R www-data:www-data /var/www/1337

WORKDIR	/var/www/1337

RUN		curl -L -o index.php https://github.com/vrana/adminer/releases/download/v4.7.8/adminer-4.7.8.php

RUN		chmod 775 index.php && chown www-data:www-data index.php

ENTRYPOINT		["php", "-S", "0.0.0.0:9090", "-t", "/var/www/1337"]