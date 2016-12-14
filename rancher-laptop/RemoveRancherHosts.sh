#!/bin/bash

#for cHost in host0 host1 host2; do
for cHost in host0 host1; do
	echo $cHost;
	eval $(docker-machine env $cHost);
	docker stop rancher-agent;
	if [ "$?" != "0" ];then
		echo "error docker stop";
	else
		docker rm rancher-agent;
		if [ "$?" != "0" ];then
			echo "error docker rm";
			exit 1;
		fi
	fi
	docker-machine stop $cHost;
	if [ "$?" != "0" ];then
		echo "error docker-machine stop";
	fi
	docker-machine rm --force $cHost;
	if [ "$?" != "0" ];then
		echo "error docker-machine rm";
		exit 1;
	fi
done
