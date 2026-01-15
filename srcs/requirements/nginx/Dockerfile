FROM debian:bookworm

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y nginx openssl && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /etc/nginx/tls && \
    openssl req -x509 -nodes -days 42 -newkey rsa:2048 \
    -keyout /etc/nginx/tls/pkey.key \
    -out /etc/nginx/tls/selfsignedcert.crt \
    -subj "/C=MO/ST=Khouribga/L=Khouribga/O=1337/OU=42/CN=mel-akar.42.fr"

RUN mkdir -p /var/www/wordpress && \
    chown -R www-data:www-data /var/www

COPY ./conf/def.conf /etc/nginx/nginx.conf

ENTRYPOINT ["nginx", "-g", "daemon off;"]