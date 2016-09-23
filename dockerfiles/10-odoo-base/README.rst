ADHOC Odoo BASE
===============
We use adhoc/odoo instead of odoo official build becaue in "adhoc/odoo" we can control odoo version and have more recet builds. But essentially, "adhoc/odoo" and "odoo" are the same. Any modification is made by extension in this repository

* Set lang es_AR
* Install some common apt packages that are required for future added repositories. We do it here to speed next images builds
* Disable auto_install for some odoo modules
* Include some scripts to add modules in next dockerfiles
* Include custom odoo.conf (and change env to odoo.conf)

Files and directories
---------------------

* We create "/opt/odoo" and take inside everthing that matters to this odoo image:
 * "/opt/odoo/etc": for odoo.conf, Published as volume
 * "/opt/odoo/data": for odoo data dir. Published as volume
 * "/opt/odoo/extra_addons": for container included addons. Not published
 * "/opt/odoo/resources": resources used by this image. Not published