
compose_path	=	 ./srcs/docker-compose.yml




up:		
	@docker compose --file $(compose_path) up

down:	
	@docker compose --file $(compose_path) down

show:	
	@docker compose --file $(compose_path) ps

exec:
	@if [ "3" -gt $(words $(MAKECMDGOALS)) ];then \
		echo "exec needs 2 args to be executed!" >> /dev/stderr; \
	else \
		docker exec -it $(word 2, $(MAKECMDGOALS)) $(word 3, $(MAKECMDGOALS)); fi

%:
	@:

prune:
	@docker  system prune -af

.PHONY: up down exec show
