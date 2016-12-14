Rancher GCE Google Compute Engine
=================================

Some Docker development files for setting up a Rancher deploy con GCE Xenial VMs


Rancher Server via Nginx SSL
============================

I provide a customized Nginx and then use it for docker-compose to startup both Rancher server and Nginx.

Prepare Nginx
-------------

* cd nginx
* Copy your cert and key into the dir as::

	./cert.pem
	./cert.key

* Modify nginx.conf and conf.d/default.conf if needed.
* Build the new Nginx docker image::

	docker build -t custom-nginx .

Docker Compose
--------------

* cd ..
* Run using provided docker-compose::

	docker-compose up -d
