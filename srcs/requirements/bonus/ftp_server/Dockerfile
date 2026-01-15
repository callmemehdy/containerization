FROM debian:bookworm

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y vsftpd && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

COPY ./conf/vsftpd.conf /etc/vsftpd.conf
COPY ./tools/ftping.sh /usr/local/bin/

RUN chmod +x /usr/local/bin/ftping.sh

ENTRYPOINT ["ftping.sh"]