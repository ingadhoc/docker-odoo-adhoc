FROM adhoc/odoo-base:9.0
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

USER root

# Get repositories & add files
WORKDIR ${RESOURCES}
COPY ./oca-sources.txt ./
RUN mkdir -p ${EXTRA_ADDONS}
RUN python get_addons.py ${EXTRA_ADDONS} --depth=1 --sources_file=oca-sources.txt
WORKDIR /

# adapat odoo for purchase_suggest
RUN sed -i '462 a \        \dom.append(("suggest", "=", False))' /usr/lib/python2.7/dist-packages/openerp/addons/stock/procurement.py

USER odoo
