FROM	debian:bullseye

RUN		apt update && apt upgrade -y && apt install -y systemd && \
		apt install -y redis

COPY	./tools/redising.sh .

RUN		chmod +x redising.sh

ENTRYPOINT \
["redis-server", "--bind", "0.0.0.0", "--port", "1234", "--protected-mode", "no"]
