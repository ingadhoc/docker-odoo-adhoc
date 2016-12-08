#!/bin/bash
if [ "$1" == "" ];then
	echo "usage: $0 <agent script from rancher server add host custom command>";
	exit 0;
fi

#for cHost in host0 host1 host2;do
for cHost in host0 host1;do
	echo $cHost;
	eval $(docker-machine env $cHost);
	docker run -d --privileged --name agent -v /var/run/docker.sock:/var/run/docker.sock -v /var/lib/rancher:/var/lib/rancher registry.unxs.io:5000/rancher/agent:v1.0.2  $1
	if [ "$?" != "0" ];then
		echo "!error run!";
	fi
	docker logs -f agent;
	docker ps --all;
done
