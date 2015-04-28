FROM debian:jessie
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

# Install some deps, lessc and less-plugin-clean-css, and wkhtmltopdf
RUN set -x; \
        apt-get update \
        && apt-get install -y --no-install-recommends \
            ca-certificates \
            curl \
            nodejs \
            npm \
            python-support \
            python-pyinotify \
        && npm install -g less less-plugin-clean-css \
        && ln -s /usr/bin/nodejs /usr/bin/node \
        && curl -o wkhtmltox.deb -SL http://nightly.odoo.com/extra/wkhtmltox-0.12.1.2_linux-jessie-amd64.deb \
        && echo '40e8b906de658a2221b15e4e8cd82565a47d7ee8 wkhtmltox.deb' | sha1sum -c - \
        && dpkg --force-depends -i wkhtmltox.deb \
        && apt-get -y install -f --no-install-recommends \
        && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false -o APT::AutoRemove::SuggestsImportant=false npm \
        && rm -rf /var/lib/apt/lists/* wkhtmltox.deb

# Install Odoo
ENV ODOO_VERSION 7.0
# ENV ODOO_RELEASE latest
ENV ODOO_RELEASE 20150428
RUN set -x; \
        curl -o odoo.deb -SL http://nightly.odoo.com/${ODOO_VERSION}/nightly/deb/openerp_${ODOO_VERSION}.${ODOO_RELEASE}_all.deb \
        && dpkg --force-depends -i odoo.deb \
        && apt-get update \
        && apt-get -y install -f --no-install-recommends \
        && rm -rf /var/lib/apt/lists/* odoo.deb

# Copy entrypoint script and Odoo configuration file
COPY ./entrypoint.sh /
COPY ./openerp-server.conf /etc/odoo/
RUN chown openerp /etc/odoo/openerp-server.conf

# Mount /var/lib/odoo to allow restoring filestore and /mnt/extra-addons for users addons
VOLUME ["/var/lib/odoo", "/mnt/extra-addons"]

# Expose Odoo services
EXPOSE 8069

# Set the default config file
ENV OPENERP_SERVER /etc/odoo/openerp-server.conf

### ADHOC customizations
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
RUN apt-get install -y swig libssl-dev
RUN pip install M2Crypto suds

# odoo argentina
RUN pip install geopy==0.95.1 BeautifulSoup

# odoo etl, infra and others
RUN pip install openerp-client-lib fabric erppeek fabtools

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

# aeroo py-uno
RUN apt-get install -y python-uno libreoffice-core
### END ADHOC customizations

# Set default user when running the container
USER openerp

ENTRYPOINT ["/entrypoint.sh"]
CMD ["openerp-server"]