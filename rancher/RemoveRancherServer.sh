#!/bin/bash
eval $(docker-machine env rancher);
docker stop rancher;
if [ "$?" != "0" ];then
	echo "error docker stop";
else
	docker rm rancher;
	if [ "$?" != "0" ];then
		echo "error docker rm";
	fi
fi
docker-machine stop rancher;
if [ "$?" != "0" ];then
	echo "error docker-machine stop";
fi
docker-machine rm --force rancher;
if [ "$?" != "0" ];then
	echo "error docker-machine rm";
	exit 1;
fi	
