FROM adhoc/odoo-adhoc:8.0
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

## Change this date when you want to make a new build
ENV REFRESHED_AT 2015-03-15

USER root

RUN git clone https://github.com/oca/ocb /mnt -b 8.0

# Copy Odoo configuration file
COPY ./run.sh /
COPY ./openerp-server.conf /etc/odoo/
RUN chown odoo /etc/odoo/openerp-server.conf

USER odoo