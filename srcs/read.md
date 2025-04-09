CMD ls | cat vs cmd ["sllsl"]

# mapping a port exposed by the container to the host port:
	docker run --name new_con_name -p [host port]  :[container port] <img_name> 
# we can use -d option to detach the container process from our terminal(if it was fourground process)


to remove the container after you stop it : we use -rm after  naming it ...