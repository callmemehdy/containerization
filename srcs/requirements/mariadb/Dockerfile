FROM debian:bookworm

COPY ./tools/mariadbing.sh /usr/local/bin/

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y mariadb-server netcat-traditional && \
    apt-get clean && rm -rf /var/lib/apt/lists/* && \
    chmod +x /usr/local/bin/mariadbing.sh

ENTRYPOINT ["mariadbing.sh"]
