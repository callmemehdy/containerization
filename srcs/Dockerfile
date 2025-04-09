FROM debian

RUN apt update && apt install build-essential -y

WORKDIR /MyApp 

COPY . .


# # EXPOSE 1337


CMD ["make"]

