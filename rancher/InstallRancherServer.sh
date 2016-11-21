#!/bin/bash
eval $(docker-machine env rancher);
docker run -d --restart=unless-stopped -p 8080:8080 --name=rancher registry.unxs.io:5000/rancher/server;
if [ "$?" != "0" ];then
	echo "error run";
	exit 1;
fi
docker logs -f rancher
