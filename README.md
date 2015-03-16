odoo-adhoc
==========

This is an image that allows you to launch the odoo v8 container linked to with another PostgreSQL container.

Configuration
-----------

Docker configuration for Odoo

Launch the PostgreSQL service:

    sudo docker run --rm -ti --name postgres \
      -v /your/local/data/directory:/var/lib/postgresql/data \
      -v /your/local/log/directory:/var/log/postgresql postgres:9.3

this command create a container named 'postgres' that will be deleted when the container stops running. This containers uses volumes to save data in /your/local/data/directory and /your/local/log/directory so you can store that information. The -ti option is to see the log of the database, replace it with -d to launch it as a daemon.

Then you should connect to the database to create a valid user for Odoo, you can use this:

    sudo docker run -it --link postgres:postgres --rm postgres \
      sh -c 'exec psql -h "$POSTGRES_PORT_5432_TCP_ADDR" \
      -p "$POSTGRES_PORT_5432_TCP_PORT" -U postgres'

and then create the odoo user with the following SQL commands:

    CREATE USER odoo WITH PASSWORD 'odoo';
    ALTER USER odoo WITH SUPERUSER;

Finally you can launch odoo connected to postgres with the following command:

    sudo docker run --rm -ti --name odoo --link postgres:odoo-db \
      -p 8069:8069 adhoc/odoo:8.0

The -ti option is to see the log of the database, replace it with -d to launch it as a daemon.

Development
-----------

If you want to use this docker image for development, you can launch it using:

    sudo docker run --rm -ti --name odoo --link postgres:odoo-db \
      -v /your/local/repo:/opt/odoo/sources/addons \
      -p 8069:8069 adhoc/odoo:8.0

where /your/local/repo is your currently cloned repository of some module. This will allow you to change the code of the module locally without the need of creating an image every time you modify the module.

If you want to update all modules in the database "database_to_update" then you can pass the complete command when you launch the container this way:

    sudo docker run --rm -ti --name odoo --link postgres:odoo-db \
      -v /your/local/repo:/opt/odoo/sources/addons \
      -p 8069:8069 adhoc/odoo:8.0 \
      sudo -H -u odoo /opt/odoo/server/odoo.py -c /opt/odoo/server/odoo.conf \
      --update=all -d database_to_update

If you also want to use a local configuration for odoo you can use volumes as well with the following option:

    sudo docker run --rm -ti --name odoo --link postgres:odoo-db \
      -v /your/local/repo:/opt/odoo/sources/addons \
      -v /your/local/odoo.conf:/opt/odoo/odoo.conf \
      -p 8069:8069 adhoc/odoo:8.0 \
      sudo -H -u odoo /opt/odoo/server/odoo.py -c /opt/odoo/odoo.conf \
      --update=all -d database_to_update

