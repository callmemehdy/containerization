FROM debian:bullseye

RUN	apt update  && apt upgrade -y && apt install -y nginx openssl

RUN	mkdir	-p /etc/nginx/tls/ && openssl req -x509 -nodes -days 42 -newkey rsa:2048 \
			-keyout /etc/nginx/tls/pkey.key -out /etc/nginx/tls/selfsignedcert.crt \
			-subj "/C=MO/ST=Khouribga/L=Khouribga/O=1337/OU=42/CN=mel-akar.42.fr"

#i should use a file to make openssl read from to fill up the certificate placeholders

# take a look on how daemons work inside a container

RUN mkdir -p /var/www/wordpress

RUN chown -R www-data:www-data /var/www


COPY ./def.conf /etc/nginx/nginx.conf


CMD ["nginx", "-g", "daemon off;"]