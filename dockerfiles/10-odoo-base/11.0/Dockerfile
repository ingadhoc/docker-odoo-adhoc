FROM adhoc/odoo-ubuntu:11.0
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

USER root

# Install some deps (if we install with no-recommends later we have some errors with pip)
RUN apt-get update \
        && apt-get install -y \
        python-pip git locales

# Generate locale (es_AR for right odoo es_AR language config, and C.UTF-8 for postgres and general locale data)
# TODO better C.UTF-8 or en_US.UTF-8?
RUN locale-gen en_US.UTF-8 && update-locale
RUN locale-gen es_AR.UTF-8 && update-locale
RUN echo 'LANG="en_US.UTF-8"' > /etc/default/locale
ENV LANG es_AR.UTF-8  
ENV LANGUAGE es_AR:es:en_US:en  
ENV LC_ALL es_AR.UTF-8

# sudo and other useful tools
RUN apt-get install -y sudo nano htop ssh wget unzip iputils-ping telnet

# psql client
RUN apt-get install -y postgresql-client-9.6

# update pip and install setuptools
RUN pip3 install --upgrade pip
RUN pip3 install --upgrade setuptools

## Install pip dependencies for adhoc used odoo repositories
# RUN pip3 install openupgradelib
RUN pip3 install git+https://github.com/OCA/openupgradelib/@master

# used by many pip packages
RUN apt-get install -y python3-dev build-essential

# instalamos algunas dependencias mencionadas en https://www.odoo.com/documentation/11.0/setup/install.html
# instalamos primero wheel para que no de error vobject
RUN pip3 install wheel
RUN pip3 install vobject qrcode
RUN apt-get -y install libldap2-dev libsasl2-dev
RUN pip3 install pyldap

# odoo-extra (por ahora no agregamos, tratamos de usar menos espacio)
# RUN apt-get install -y python-matplotlib font-manager
# RUN pip3 install simplejson

# odoo argentina (nuevo modulo de FE)
# TODO borrar o dejar
# RUN apt-get install -y swig libffi-dev libssl-dev python-m2crypto python-httplib2
# este deberiamos borrarlo porque tarda, es para pyOpenSSL que no deberia ser necesario si usamos pyafipws

# odoo-saas (para pycurl)
# RUN apt-get install -y libcurl4-openssl-dev parallel

# aeroo direct print
RUN apt-get install -y libcups2-dev

## Clean python 2.7
RUN apt-get -yqq purge python2.7

## Clean apt-get (copied from odoo)
RUN apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false -o APT::AutoRemove::SuggestsImportant=false
RUN apt-get clean
RUN rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Make auto_install = False for various modules
RUN sed  -i  "s/'auto_install': True/'auto_install': False/" /usr/lib/python3/dist-packages/odoo/addons/procurement_jit/__manifest__.py

# create directory for odoo files
RUN mkdir -p /opt/odoo
RUN chown -R odoo /opt/odoo

# changing user is required by odoo which won't start with root
USER odoo
ENV ETC_DIR /opt/odoo/etc
ENV DATA_DIR /opt/odoo/data
ENV CUSTOM_ADDONS /opt/odoo/custom-addons
ENV EXTRA_ADDONS /opt/odoo/extra-addons
RUN mkdir -p ${ETC_DIR}
RUN mkdir -p ${DATA_DIR}
RUN mkdir -p ${CUSTOM_ADDONS}
RUN mkdir -p ${EXTRA_ADDONS}

USER root

# Mount persistent directories
VOLUME ["${DATA_DIR}", "${CUSTOM_ADDONS}"]
# no tendriamos porque montar "/opt/odoo/etc" ya que la idea es que se reconstruya el conf dinamicamente

# rplace  entrypoint script and replace Odoo configuration file (new name)
ENV RESOURCES /opt/odoo/resources/
COPY ./resources/ ${RESOURCES}
RUN chown -R odoo ${RESOURCES}

# change entrypoint
ENTRYPOINT ["/opt/odoo/resources/entrypoint.sh"]
CMD ["odoo.py"]

# Variables used by the launch scripts
ENV \
    ODOO_SERVER=odoo.py \
    ODOO_CONF=/opt/odoo/etc/odoo.conf \
    FIXDBS=False \
    WAIT_PG=True \
    PG_MAX_CONNECTIONS=100 \
    # odoo conf variables
    MAX_CRON_THREADS=1 \
    SMTP_SERVER=localhost \
    SMTP_PORT=25 \
    SMTP_SSL=False \
    SMTP_USER=False \
    SMTP_PASSWORD=False \
    UNACCENT=True \
    PGUSER=odoo \
    PGPASSWORD=odoo \
    PGHOST=db \
    WORKERS=6 \
    ADMIN_PASSWORD=admin \
    SERVER_MODE=test \
    PROXY_MODE=True \
    WITHOUT_DEMO=True \
    DBFILTER=.* \
    LIMIT_TIME_CPU=1600 \
    LIMIT_TIME_REAL=3200 \
    LIMIT_MEMORY_HARD=2684354560 \
    LIMIT_MEMORY_SOFT=2147483648 \
    AEROO_DOCS_HOST=aeroo \
    AFIP_HOMO_PKEY_FILE=/opt/odoo/backups/homo.pkey \
    AFIP_HOMO_CERT_FILE=/opt/odoo/backups/homo.cert \
    AFIP_PROD_PKEY_FILE=/opt/odoo/backups/prod.pkey \
    AFIP_PROD_CERT_FILE=/opt/odoo/backups/prod.cert \
    DB_TEMPLATE=template1

# odoo server use this env var as default conf, check if v9 or v10 rename it
ENV ODOO_RC $ODOO_CONF

USER odoo
