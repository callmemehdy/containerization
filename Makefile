##########################################################
##                                               		##
##     M E H D I  E L  A K A R Y  - GOATEDINCEPTION     ##
##           						             		##
##########################################################

compose_path	=	 ./srcs/docker-compose.yml

up:		
	@docker compose --file $(compose_path) up --build

down:	
	@docker compose --file $(compose_path) down

show:	
	@docker compose --file $(compose_path) ps -a

exec:
	@if [ "3" -gt $(words $(MAKECMDGOALS)) ];then \
		echo "exec needs 2 args to be executed!" >> /dev/stderr; \
	else \
		docker exec -it $(word 2, $(MAKECMDGOALS)) $(word 3, $(MAKECMDGOALS)); fi

volume_dir:
	@if [ ! -d ../data ];then \
		mkdir -p ../data/wordpress && mkdir -p ../data/mariadb; \
	fi

remove_volumes:
	@rm -rf ../data/*/*


prune:
	@docker  system prune -af

.PHONY: \
up down exec show prune remove_volumes

%:
	@:
