Rancher GCE Nginx Docker Proxy 
==============================

Docker image based on nginx.

Runs via supervisord nginx and cron. 

cron is used to reconfigure nginx if changes are picked up from the host node.

Designed for use with Rancher on a GCE Ubuntu VM.

Uses AdHoc Rancher Catalog Odoo containers what have the special ENV variables for
configuring nginx proxy: VIRTUAL_HOST and VIRTUAL_PORT

VIRTUAL_HOST is the DNS host name.
VIRTUAL_PORT is a comma seperated list of ports to proxy. In this case hardcoded in .tpl and code
for Odoo http and Odoo chat/IM traffic.

Shortly
-------

We will add more ENV vars for SSL configuration.
And SSL .tpl files.

dockprox program
----------------

Very simple unix socket reader and JSON parser are used together with a simple template
system to create nginx configuration file.

Designed to limit dependencies and be very fast and lightweight.

In the provided case for an Odoo ERP proxy.

Uses io.rancher labels and other docker API data to configure.

Easy to modify
--------------

With very basic C language skills dockprox.c is easy to change to suit your own needs. You should
be able to figure out how to parse any Docker or Rancher label name/value data.

Same with the template sections and the conf .tpl files.

nginx/ dir contains /etc/nginx/conf.d/docker.conf configuration section
templates server and upstream.

nginx/nginx.conf and nginx/default.conf can be easily modified 
and then the Docker image rebuit.

Using
-----

Edit dockprox.c and .tpl files, then::

    make
    docker build -t nginx-dockprox .
    docker run --restart unless-stopped --name nginx-dockprox -p 80:80 -v /var/run/docker.sock:/var/run/docker.sock:ro -d nginx-dockprox


Help
----

Contact support@unxs.io for free limited support.
