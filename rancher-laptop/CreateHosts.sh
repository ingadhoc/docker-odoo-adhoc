#!/bin/bash

#for cHost in host0 host1 host2; do
for cHost in host0 host1; do
	echo $cHost;
	docker-machine create -d virtualbox --virtualbox-boot2docker-url file:///home/unxs/.docker/machine/cache/rancheros.iso $cHost;
	if [ "$?" != "0" ];then
		echo "error create 0. trying to fix...";
		docker-machine regenerate-certs --force $cHost;
		if [ "$?" != "0" ];then
			echo "error create 1.";
			exit 1;
		fi
	fi

	eval $(docker-machine env $cHost);
	docker pull registry.unxs.io:5000/rancher/agent:v1.0.2;
	if [ "$?" != "0" ];then
		echo "error pull 0";
		exit 2;
	fi
done
