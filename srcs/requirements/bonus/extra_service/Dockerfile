FROM	debian:bullseye

RUN		apt update && apt upgrade -y && apt install -y build-essential

RUN		mkdir ft_irc_protocol

COPY	./tools ft_irc_protocol/

WORKDIR /ft_irc_protocol

ENTRYPOINT ["make", "run"]