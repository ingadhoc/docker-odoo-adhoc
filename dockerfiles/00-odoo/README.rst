About this fork
===============

This is a fork of odoo docker official image. We make the fork for this reasons:
* We want to use more recent nighty odoo builds and official repository is updated only after some months
* We are more used to work with ubuntu, so we change to ubuntu as base image, for that, this adjustaments:
    * replace wkhtmltox version
    * not install python-support as not anymore on ubuntu 16.04
* On default odoo, as there are no locales, when creating a new database and loading language  (for eg. es_ar), no correct language info is loaded (for eg date format). It is also much easier to create locales on ubunto rather than in debian

Tags
----

* 8.0
* 9.0

# About this Repo

This is the Git repo of the official Docker image for [Odoo](https://registry.hub.docker.com/_/odoo/). See the Hub page for the full readme on how to use the Docker image and for information regarding contributing and issues.

The full readme is generated over in [docker-library/docs](https://github.com/docker-library/docs), specifically in [docker-library/docs/odoo](https://github.com/docker-library/docs/tree/master/odoo).
