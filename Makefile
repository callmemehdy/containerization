compose_path = ./srcs/docker-compose.yml

up: volume_dir
	@docker compose --file $(compose_path) up --build

down:
	@docker compose --file $(compose_path) down -v

show:
	@docker compose --file $(compose_path) ps -a

exec:
	@if [ "3" -gt $(words $(MAKECMDGOALS)) ]; then \
		echo "exec needs 2 args to be executed!" >> /dev/stderr; \
	else \
		docker exec -it $(word 2, $(MAKECMDGOALS)) $(word 3, $(MAKECMDGOALS)); \
	fi

volume_dir:
	@if [ ! -d ../data ]; then \
		mkdir -p ../data/wordpress ../data/mariadb; \
	fi

remove_volumes:
	@rm -rf ../data/*/*

prune:
	@docker system prune -af

push:
	@git add . && read -p "Enter a commit message: " c && git commit -m "$$c" && git push

re: down up


.PHONY: up down exec show prune remove_volumes

%:
	@:
