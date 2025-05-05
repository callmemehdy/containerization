FROM	debian:bullseye

RUN		apt update  && apt upgrade -y  && apt install -y build-essential

COPY	./conf/miniminiserver.cpp .

RUN		c++ miniminiserver.cpp -o apache

ENTRYPOINT	["./apache"]