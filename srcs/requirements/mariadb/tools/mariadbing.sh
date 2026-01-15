#!/bin/bash

service mariadb start

mariadb -h localhost -e "CREATE DATABASE IF NOT EXISTS $MARIADB_DB;"
mariadb -h localhost -e "CREATE USER IF NOT EXISTS '$MARIADB_USER'@'%' IDENTIFIED BY '$MARIADB_PASSWORD';"
mariadb -h localhost -e "GRANT ALL PRIVILEGES ON $MARIADB_DB.* TO '$MARIADB_USER'@'%';"
mariadb -h localhost -e "FLUSH PRIVILEGES;"

mariadb-admin shutdown

exec mariadbd-safe --bind-address=0.0.0.0 --port=3306 --datadir='/var/lib/mysql'