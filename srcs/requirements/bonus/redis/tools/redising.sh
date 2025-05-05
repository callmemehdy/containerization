#!/bin/bash



# sed -i 's#bind 127.0.0.1 ::1#bind 0.0.0.0 ::0#' /etc/redis/redis.conf
# sed -i 's#protected-mode yes#protected-mode no#' /etc/redis/redis.conf
# sed -i 's#port 6379#port 1234#' /etc/redis/redis.conf

redis-server --bind 0.0.0.0 --port 1234 --protected-mode no
