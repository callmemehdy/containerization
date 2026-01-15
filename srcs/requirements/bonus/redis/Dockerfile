FROM debian:bookworm

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y redis netcat-traditional && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["redis-server", "--bind", "0.0.0.0", "--port", "1234", "--protected-mode", "no"]
