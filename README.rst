Odoo Docker Compose and Images
==============================
This repository contains a set of dockerfiles to build different docker images used by ADHOC.
It also provides a docker-compose file to use odoo in a simple way.

Requirements
============

To use this docker compose file need:

* docker-compose "pip install docker-compose"
* docker-engine "sudo curl -sSL https://get.docker.com/ | sh"
* we also recommend this so you can use docker without sudo

    * sudo groupadd docker
    * sudo gpasswd -a $USER docker
    * sudo service docker restart && newgrp docker
* clone this repository "git clone git@github.com:ingadhoc/docker-odoo-adhoc.git"

Run and usefull commands
========================

* Run "docker-compose up"
* Odoo should be ready to be used on http://localhost:8069/


Links
=====
* `Images readme <dockerfiles/README.rst/>`_
* `More about docker compose <docs/docker_compose.rst/>`_
* `Roadmap / Known Issues <docs/TODO.rst/>`_
* `Inspired by <docs/inspired_by.rst/>`_
