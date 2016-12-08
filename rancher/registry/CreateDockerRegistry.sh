#!/bin/bash

#PURPOSE
#	quick and dirty docker registry v2 creation
#SSL
#	You will need DNS (or /etc/hosts) and valid cert/key
#	for rancher and most other out of box integration.

if [ ! -d /data/docker-registry ];then
	mkdir -p /data/docker-registry;
	mkdir /data/docker-registry-auth;
	mkdir /data/docker-registry-certs;
	echo "data dirs created";
	echo "you need to copy your ssl cert and key into docker-registry-certs";
fi

#some problems with this, not required for private network.
#  -e "REGISTRY_AUTH=htpasswd" \
#  -e "REGISTRY_AUTH_HTPASSWD_REALM=Registry Realm" \
#  -e "REGISTRY_AUTH_HTPASSWD_PATH=/auth/htpasswd" \
docker run -d -p 5000:5000 --restart=always --name docker-registry \
  -v /data/docker-registry:/var/lib/registry \
  -v /data/docker-registry-auth:/auth \
  -v /data/docker-registry-certs:/certs \
  -e "REGISTRY_HTTP_TLS_CERTIFICATE=/certs/fullchain.pem" \
  -e "REGISTRY_HTTP_TLS_KEY=/certs/registry.key" \
  registry:2
