FROM adhoc/odoo-adhoc:9.0.x
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

USER root

# predefined parameters vals
ENV CUSTOM_ADDONS /mnt/extra-addons
ENV ADMIN_PASS admin
ENV SERVER_WIDE_MODULES web

COPY ./resources/openerp-server.conf /etc/odoo/
RUN chown odoo /etc/odoo/openerp-server.conf

# Create directories for addons and odoo data
ENV SOURCES_DIR /opt/odoo/sources
ENV EXTRA_ADDONS /opt/odoo/extra-addons
RUN mkdir -m a+rw -p ${SOURCES_DIR}
RUN mkdir -m a+rw -p ${EXTRA_ADDONS}

# Get repositories & add files
WORKDIR ${SOURCES_DIR}
ADD resources/get_addons.py ./
ADD resources/sources.txt ./
RUN python get_addons.py ${EXTRA_ADDONS} --depth=1 --sources_file=sources.txt
WORKDIR /

# para pyafipws
RUN pip install -r https://raw.githubusercontent.com/reingart/pyafipws/master/requirements.txt
RUN pip install git+https://github.com/reingart/pyafipws@master
RUN chmod 777 -R /usr/local/lib/python2.7/dist-packages/pyafipws/

USER odoo
