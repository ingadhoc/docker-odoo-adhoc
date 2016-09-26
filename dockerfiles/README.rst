Odoo Images
===========

Set of odoo images, starting from odoo-base, later odoo-oca, etc. The number prefix (eg. 10-) is only to have directories sorted as the images depends on each other


Images summary
--------------

Each of this images has a tag 8.0 and 9.0 and can be found on docker registry. Images:

* adhoc/odoo-ubuntu: fork of odoo docker official image but with ubuntu and controlled (by adhoc) nightly build date
* adhoc/odoo-base: base image with custom odoo.conf, entrypoint, packages and some scripts
* adhoc/odoo-oca: extends odoo-base and includes all oca repositories (and some others) used by adhoc
* adhoc/odoo-ar: Extends odoo-oca and includes repositories commonly used on argentina (ingadhoc repositories)


Usefull Environment Variables
-----------------------------

* ADMIN_PASSWORD: will be used on the odoo.conf (default 'admin')
* WORKERS: odoo workers (default '4')


For each image
==============

Build it
--------

Replace this variables:
* image_sufix = base/oca/ar
* image_dir = 10-odoo-base/20-odoo-oca/30-odoo-ar
* odoo_version = 8.0/9.0

Build:
* docker build $image_dir/$odoo_version/ -t odoo-$image_sufix:$odoo_version

Eg:
* docker build 10-odoo-base/8.0/ -t odoo-base:8.0
* docker build 20-odoo-oca/8.0/ -t odoo-oca:8.0
* docker build 30-odoo-ar/8.0/ -t odoo-ar:8.0


Try it
------
* run postgres:
  * docker run --rm -e POSTGRES_USER=odoo -e POSTGRES_PASSWORD=odoo --name db postgres:9.5
* run odoo:
  * docker build . -t odoo-base:8.0
  * docker run --rm -ti -p 8069:8069 -p 8072:8072 --link db:db --name odoo odoo-$image_sufix:$odoo_version
