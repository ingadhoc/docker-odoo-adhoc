Rancher GCE (Google Compute Engine)
===================================

Some Docker development files for setting up a Rancher deploy with GCE Xenial VMs

GCE VM Post Start Setup
-----------------------

After starting your GCE Ubuntu Xenial instance you will need to install docker::

    apt-get update && apt-get install -y python-pip git
    pip install --upgrade pip
    locale-gen en_US.UTF-8 && update-locale
    echo 'LANG="en_US.UTF-8"' > /etc/default/locale
    pip install --upgrade pip
    pip install --upgrade docker-compose
    curl -sSL https://get.docker.com/ | sh
    docker ps
	


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

Rancher Server
--------------

* Open browser to rancher web console and set the rancher server settings host registration.
* Set the access control, e.g. Local Authentication.
* Start another VM and run the infrastructure add host custom command that should look like::

	sudo docker run -d --privileged -v /var/run/docker.sock:/var/run/docker.sock -v /var/lib/rancher:/var/lib/rancher rancher/agent:v1.1.0 https://rancher.your.io/v1/scripts/029236FA12EEADA40836:1481727600000:ouXxT48Ydl5uEId60jzOTvjhs
