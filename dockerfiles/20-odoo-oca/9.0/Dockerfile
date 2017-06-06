FROM adhoc/odoo-base:9.0
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

USER root

# Get repositories & add files
WORKDIR ${RESOURCES}
COPY ./oca-sources.txt ./
RUN mkdir -p ${EXTRA_ADDONS}
RUN python get_addons.py ${EXTRA_ADDONS} --depth=1 --sources_file=oca-sources.txt
WORKDIR /

# parche por error introducido aca https://github.com/OCA/web/commit/e37db6412aa4ca85c8ad1635c49e9150a202f5af que hace que si se desactiva el create edit de manera global, no importa que en los pickings se fuerce que permita, no te deja crear registro, por ejemplo esto pasaba en boggio
WORKDIR ${EXTRA_ADDONS}
RUN git clone https://github.com/OCA/web -b 9.0 oca-web && cd oca-web && git checkout bdb6c994ad4fb65ffb2f01aaa2adfd5a0d22baba && cd ../
WORKDIR /

USER odoo
