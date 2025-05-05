FROM debian:bullseye

COPY ./tools/mariadbing.sh .

RUN apt update && apt upgrade -y && apt install -y systemd && \
	apt install -y mariadb-server mariadb-client

RUN chmod +x mariadbing.sh

ENTRYPOINT ["./mariadbing.sh"]

