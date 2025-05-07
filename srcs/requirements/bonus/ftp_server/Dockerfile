FROM	debian:bullseye

RUN		apt update  && apt upgrade -y && apt install vsftpd -y

COPY	./conf/vsftpd.conf	/etc/vsftpd.conf

COPY	./tools/ftping.sh .

RUN		chmod +x ftping.sh

ENTRYPOINT	["./ftping.sh"]