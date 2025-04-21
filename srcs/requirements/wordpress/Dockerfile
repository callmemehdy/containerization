FROM debian:bullseye

#unshare syscall

RUN apt update && apt upgrade -y && apt install curl -y &&  \
	curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar && \
	chmod +x wp-cli.phar && \
	mv wp-cli.phar /bin/local/bin/wp

CMD ["./tools/wordpressing.sh"]

