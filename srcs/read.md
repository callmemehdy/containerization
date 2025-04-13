CMD ls | cat vs cmd ["sllsl"]

# mapping a port exposed by the container to the host port:
	docker run --name new_con_name -p [host port]  :[container port] <img_name> 
# we can use -d option to detach the container process from our terminal(if it was fourground process)


to remove the container after you stop it : we use -rm after  naming it ...

to see the docker image layers use docker history [img name] or use docker inspect to see img metadata.....

the CMD instruction can be overriden using docker run parameters unlike the ENTRYPOINT instruction

 docker login --username=meehd