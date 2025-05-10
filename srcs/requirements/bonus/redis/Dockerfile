FROM	debian:bullseye

RUN		apt update && apt upgrade -y && \
		apt install -y redis netcat

ENTRYPOINT \
["redis-server", "--bind", "0.0.0.0", "--port", "1234", "--protected-mode", "no"]
