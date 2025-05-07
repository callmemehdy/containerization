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

i should read some article about docker-machine

docker system info | grep Storage




// see the nginx layers

<!-- We have an image (nginx) to play with, so next, let's checkout it's layers. We can inspect image layers by either running docker inspect on the image and checking the GraphDriver fields or by going through /var/lib/docker/overlay2 directory where all image layers are stored. So, let's do both and see what's inside: -->

<!-- ~ $ cd /var/lib/docker/overlay2
~ $ ls -l
total 0
drwx------. 4 root root     55 Feb  6 19:19 3d963d191b2101b3406348217f4257d7374aa4b4a73b4a6dd4ab0f365d38dfbd
drwx------. 3 root root     47 Feb  6 19:19 410c05aaa30dd006fc47d8c23ba0d173c6d305e4d93fdc3d9abcad9e78862b46
drwx------. 4 root root     72 Feb  6 19:19 685374e39a6aac7a346963bb51e2fc7b9f5e2bdbb5eac6c76ccdaef807abc25e
brw-------. 1 root root 253, 0 Jan 31 18:15 backingFsBlockDev
drwx------. 4 root root     72 Feb  6 19:19 d487622ece100972afba76fda13f56029dec5ec26ffcf552191f6241e05cab7e
drwx------. 4 root root     72 Feb  6 19:19 fb18be50518ec9b37faf229f254bbb454f7663f1c9c45af9f272829172015505
drwx------. 2 root root    176 Feb  6 19:19 l

~ $ tree 3d963d191b2101b3406348217f4257d7374aa4b4a73b4a6dd4ab0f365d38dfbd/
3d963d191b2101b3406348217f4257d7374aa4b4a73b4a6dd4ab0f365d38dfbd/
├── diff
│   └── docker-entrypoint.d
│       └── 20-envsubst-on-templates.sh
├── link
├── lower
└── work -->

<!-- ~ $ docker inspect nginx | jq .[0].GraphDriver.Data
{
  "LowerDir": "/var/lib/docker/overlay2/fb18be50518ec9b37faf229f254bbb454f7663f1c9c45af9f272829172015505/diff:
    /var/lib/docker/overlay2/d487622ece100972afba76fda13f56029dec5ec26ffcf552191f6241e05cab7e/diff:
    /var/lib/docker/overlay2/685374e39a6aac7a346963bb51e2fc7b9f5e2bdbb5eac6c76ccdaef807abc25e/diff:
    /var/lib/docker/overlay2/410c05aaa30dd006fc47d8c23ba0d173c6d305e4d93fdc3d9abcad9e78862b46/diff",
  "MergedDir": "/var/lib/docker/overlay2/3d963d191b2101b3406348217f4257d7374aa4b4a73b4a6dd4ab0f365d38dfbd/merged",
  "UpperDir": "/var/lib/docker/overlay2/3d963d191b2101b3406348217f4257d7374aa4b4a73b4a6dd4ab0f365d38dfbd/diff",
  "WorkDir": "/var/lib/docker/overlay2/3d963d191b2101b3406348217f4257d7374aa4b4a73b4a6dd4ab0f365d38dfbd/work"
} -->

docker inspect nginx | jq ".[0].GraphDriver.Data"

jq is a json file parser(json processor)             


should the http rfc for a while


hen a user logs in, vsftpd changes the root of their session to the directory specified by secure_chroot_dir (e.g., /var/run/vsftpd/empty).
The user’s view of the file system is restricted to this directory and its subdirectories. They cannot navigate "up" to parent directories or access other parts of the server’s file system.
For example, if secure_chroot_dir=/var/run/vsftpd/empty, the user is locked into that directory and sees it as their root (/).