Rancher GCE Nginx Docker Proxy 
==============================


Custom Nginx auto configuration program: docker. Monitors /var/run/docker.sock
and generates a new /etc/nginx/conf.d/default.conf and reloads nginx on changes.

Uses io.rancher labels and other docker API data to configure.

Just started
------------
