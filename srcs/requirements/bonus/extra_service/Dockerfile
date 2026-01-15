FROM debian:bookworm

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential && \
    apt-get clean && rm -rf /var/lib/apt/lists/* && \
    mkdir ft_irc_protocol

COPY ./tools ft_irc_protocol/

WORKDIR /ft_irc_protocol

ENTRYPOINT ["make", "run"]