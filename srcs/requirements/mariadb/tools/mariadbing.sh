#!/bin/bash


service mariadb start
 

mariadb -e "create database if not exists $MARIADB_DB ;"
mariadb -e "create user if not exists $MARIADB_USER@'%' identified by '$MARIADB_PASSWORD';"
mariadb -e "grant all privileges on * . * to $MARIADB_USER@'%' identified by '$MARIADB_PASSWORD';"
mariadb -e "flush privileges;"


mariadb-admin -u root -p$MARIADB_PASSWORD shutdown


mariadbd-safe --bind-address=0.0.0.0 --port=3306 --datadir='/var/lib/mysql'
