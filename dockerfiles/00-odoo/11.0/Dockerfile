FROM ubuntu:17.10
MAINTAINER Juan Jose Scarafia <jjs@adhoc.com.ar>

# Generate locale C.UTF-8 for postgres and general locale data
ENV LANG C.UTF-8

# Install some deps, lessc and less-plugin-clean-css, and wkhtmltopdf
RUN set -x; \
        apt-get update \
        && apt-get install -y --no-install-recommends \
            ca-certificates \
            curl \
            node-less \
            python3-pip \
            python3-setuptools \
            python3-renderpm \
            libssl1.0-dev \
            xz-utils \
        && apt-get -yqq purge python2.7 '*-dev' \
        && apt-get -yqq autoremove \
        && rm -Rf /var/lib/apt/lists/*

# INSTALL wkhtmltox
ARG WKHTMLTOPDF_VERSION=0.12.4
ARG WKHTMLTOPDF_CHECKSUM='049b2cdec9a8254f0ef8ac273afaf54f7e25459a273e27189591edc7d7cf29db'
RUN curl -SLo wkhtmltox.tar.xz https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/${WKHTMLTOPDF_VERSION}/wkhtmltox-${WKHTMLTOPDF_VERSION}_linux-generic-amd64.tar.xz \
    && echo "${WKHTMLTOPDF_CHECKSUM}  wkhtmltox.tar.xz" | sha256sum -c - \
    && tar --strip-components 1 -C /usr/local/ -xf wkhtmltox.tar.xz \
    && rm wkhtmltox.tar.xz \
    && wkhtmltopdf --version

# Install Odoo
ENV ODOO_VERSION 11.0
ENV ODOO_RELEASE 20180114
RUN set -x; \
        curl -o odoo.deb -SL http://nightly.odoo.com/${ODOO_VERSION}/nightly/deb/odoo_${ODOO_VERSION}.${ODOO_RELEASE}_all.deb \
        && dpkg --force-depends -i odoo.deb \
        && apt-get update \
        && apt-get -y install -f --no-install-recommends \
        && apt-get purge -yqq '*-dev' \
        && apt-get -yqq autoremove \
        && rm -rf /var/lib/apt/lists/* odoo.deb

# Copy entrypoint script and Odoo configuration file
COPY ./entrypoint.sh /
COPY ./odoo.conf /etc/odoo/
RUN chown odoo /etc/odoo/odoo.conf

# Mount /var/lib/odoo to allow restoring filestore and /mnt/extra-addons for users addons
RUN mkdir -p /mnt/extra-addons \
        && chown -R odoo /mnt/extra-addons
VOLUME ["/var/lib/odoo", "/mnt/extra-addons"]

# Expose Odoo services
EXPOSE 8069 8071

# Set the default config file
ENV ODOO_RC /etc/odoo/odoo.conf

# Set default user when running the container
USER odoo

ENTRYPOINT ["/entrypoint.sh"]
CMD ["odoo"]
