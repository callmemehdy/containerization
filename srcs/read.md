CMD ls | cat vs cmd ["sllsl"]

# mapping a port exposed by the container to the host port:
	docker run --name new_con_name -p [host port]  :[container port] <img_name> 
# we can use -d option to detach the container process from our terminal(if it was fourground process)


to remove the container after you stop it : we use -rm after  naming it ...

to see the docker image layers use docker history [img name] or use docker inspect to see img metadata.....

the CMD instruction can be overriden using docker run parameters unlike the ENTRYPOINT instruction

 docker login --username=meehd

 The -d flag stands for "driver".

So when you do:

docker network create -d bridge my-network
You're saying:
		"Hey Docker, create a new network using the bridge driver."
when we dont specify -d flag automatically the bridge driver get in

types of docker network drivers: 
								bridge
								host
								overlay
								IPvLAN
								macvlan

 docker network connect demo-network container2
 docker network disconnect demo-network container2

 docker compose specs: 

networks:
  db:
    driver: bridge
