#!/bin/bash
docker-machine create -d virtualbox --virtualbox-boot2docker-url file:///home/unxs/.docker/machine/cache/rancheros.iso rancher;
if [ "$?" != "0" ];then
	echo "error create 1. trying to fix...";
	sleep 5;
	docker-machine regenerate-certs --force rancher;
	if [ "$?" != "0" ];then
		echo "error create 2";
		exit 1;
	fi
fi
sleep 5;
eval $(docker-machine env rancher);
docker run -d --restart=unless-stopped -p 8080:8080 --name=rancher registry.unxs.io:5000/rancher/server;
if [ "$?" != "0" ];then
	echo "error run";
	exit 1;
fi
#docker logs -f rancher
docker ps -all;
