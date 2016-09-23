FROM odoo:8.0-ubuntu
#FROM adhoc/odoo:8.0-ubuntu
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

USER root

# Generate locale (es_AR for right odoo es_AR language config, and C.UTF-8 for postgres and general locale data)
# TODO better C.UTF-8 or en_US.UTF-8?
RUN locale-gen en_US.UTF-8 && update-locale
RUN locale-gen es_AR.UTF-8 && update-locale
RUN echo 'LANG="es_AR.UTF-8"' > /etc/default/locale

# Install some deps
RUN apt-get update \
        && apt-get install -y \
        python-pip git

# Workers and longpolling dependencies
RUN apt-get install -y python-gevent
RUN pip install psycogreen

## Install pip dependencies for adhoc used odoo repositories

# used by many pip packages
RUN apt-get install -y python-dev build-essential

# odoo-extra
RUN apt-get install -y python-matplotlib font-manager

# odoo argentina (nuevo modulo de FE)
#RUN apt-get install -y swig libffi-dev libssl-dev python-m2crypto python-httplib2
# este deberiamos borrarlo porque tarda, es para pyOpenSSL que no deberia ser necesario si usamos pyafipws

# aeroo direct print
RUN apt-get install -y libcups2-dev

## Clean apt-get (copied from odoo)
RUN apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false -o APT::AutoRemove::SuggestsImportant=false
RUN apt-get clean
RUN rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Make auto_install = False for various modules
RUN sed  -i  "s/'auto_install': True/'auto_install': False/" /usr/lib/python2.7/dist-packages/openerp/addons/im_odoo_support/__openerp__.py

RUN sed  -i  "s/'auto_install': True/'auto_install': False/" /usr/lib/python2.7/dist-packages/openerp/addons/base_import/__openerp__.py

RUN sed  -i  "s/'auto_install': True/'auto_install': False/" /usr/lib/python2.7/dist-packages/openerp/addons/portal/__openerp__.py

# create directory for odoo files
RUN mkdir -p /opt/odoo
RUN chown -R odoo /opt/odoo

# changing user is required by odoo which won't start with root
USER odoo
RUN mkdir -p /opt/odoo/etc
RUN mkdir -p /opt/odoo/data
#RUN mkdir -p /opt/odoo/var
ENV CUSTOM_ADDONS /opt/odoo/custom-addons
ENV EXTRA_ADDONS /opt/odoo/extra-addons
RUN mkdir -p ${CUSTOM_ADDONS}
RUN mkdir -p ${EXTRA_ADDONS}

USER root

# Mount persistent directories
VOLUME ["/opt/odoo/etc", "/opt/odoo/data", "/opt/odoo/extra-addons"]
# VOLUME ["/opt/odoo/var", "/opt/odoo/etc", "/opt/odoo/additional_addons", "/opt/odoo/data"]

# rplace  entrypoint script and replace Odoo configuration file (new name)
COPY ./resources/ /opt/odoo/resources
RUN chown -R odoo /opt/odoo/resources/

# Set the default config file
ENV ODOO_CONF /opt/odoo/etc/odoo.conf

# change entrypoint
ENTRYPOINT ["/opt/odoo/resources/entrypoint.sh"]

USER odoo
