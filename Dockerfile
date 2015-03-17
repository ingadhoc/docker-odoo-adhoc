FROM odoo:8.0
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

## Change this date when you want to make a new build
ENV REFRESHED_AT 2015-03-15

USER root

# Install some deps
RUN apt-get update \
        && apt-get install -y \
        python-pip git

# Workers and longpolling dependencies
RUN apt-get install -y python-gevent
RUN pip install psycogreen

## Install pip dependencies for adhoc used odoo repositories

# used by many pip packages
RUN apt-get install -y python-dev

# odoo adhoc
RUN apt-get install -y swig
RUN pip install M2Crypto suds

# odoo argentina
RUN pip install geopy==0.95.1 BeautifulSoup

# odoo etl, infra and others
RUN pip install openerp-client-lib fabric erppeek

# oca reports
RUN pip install xlwt

# odoo kineses
RUN pip install xlrd

# oca partner contacts
RUN pip install unicodecsv

# aeroo direct print
RUN apt-get install -y libcups2-dev
RUN pip install git+https://github.com/aeroo/aeroolib.git@master
RUN pip install pycups==1.9.68

# akretion/odoo-usability
RUN pip install BeautifulSoup4

# OCA knowledge
RUN pip install python-magic

# odoo support
RUN pip install erppeek

# Permisos a odoo y al resto de los usuarios para ver archivos
RUN chmod 666 -R /etc/odoo
RUN chown odoo -R /etc/odoo
RUN chmod 666 -R /var/lib/odoo
RUN chown odoo -R /var/lib/odoo

USER odoo