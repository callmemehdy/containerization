#!/bin/bash


service mariadb start -uroot -p$MARIADB_PASSWORD_ROOT
 

mariadb -e "create database if not exists $MARIADB_DB;"
mariadb -e "create user if not exists $MARIADB_USER@'%' identified by '$MARIADB_PASSWORD';"
mariadb -e "grant all privileges on $MARIADB_DB.* to $MARIADB_USER@'%'"
mariadb -e "flush privileges;"
mariadb -e "alter user 'root'@'localhost' identified by '$MARIADB_ROOT_PASS';"




mariadb-admin -uroot -p$MARIADB_ROOT_PASS shutdown

mariadbd-safe --bind-address=0.0.0.0 --port=3306 --datadir='/var/lib/mysql'
