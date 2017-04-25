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

* We create "/opt/odoo" and take inside everthing that matters to this odoo image (dir / VAR):
 * "/opt/odoo/etc": for odoo.conf, Published as volume
 * "/opt/odoo/data": for odoo data dir. Published as volume
 * "/opt/odoo/extra_addons / EXTRA_ADDONS": for container included addons. Not published
 * "/opt/odoo/custom_addons / CUSTOM_ADDONS": for user custom addons. Published as volume
 * "/opt/odoo/resources / RESOURCES": resources used by this image. Not published

Environment variables
---------------------

* Commonly used:
  * ADMIN_PASSWORD: will be used on the odoo.conf (default 'admin')
  * CUSTOM_ADDONS: location for addons added outside image (default '/opt/odoo/custom-addons')
  * WORKERS: odoo workers (default '4')
  * DATABASE: database for odoo conf (none set by default, if you set a dabase name then odoo will automatically create this db on start)
* Advance use:
  * UNACCENT: to enable or disable unaccent functionality (default 'True')
* Shouldn't be changed:
  * DB_TEMPLATE: odoo template db (default 'tempalte1')
  * ODOO_CONF: odoo configuaration file location (default '/opt/odoo/etc/odoo.conf')
  * EXTRA_ADDONS: location used for addons added by images (default '/opt/odoo/extra-addons')
  * PGUSER: postgres user (default 'odoo')
  * PGPASSWORD: postgres pass (default 'odoo')
  * ODOO_SERVER: run server command (default 'odoo.py')
  * RESOURCES: resources (common files used by this image builds) location (default '/opt/odoo/resources/')
