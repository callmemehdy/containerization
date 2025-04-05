FROM ubuntu:20.04

WORKDIR /MyApp 

COPY . .

RUN apt update && apt install -y build-essential

# EXPOSE 1337

CMD ["make"]
