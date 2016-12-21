FROM nginx
USER root
RUN apt-get update && apt-get -y -q --no-install-recommends install cron libcurl3 && apt-get clean \
	&& rm -r /var/lib/apt/lists/*
RUN mkdir -p /var/local/dockprox
COPY ./dockprox /usr/sbin/dockprox
COPY ./upstream.conf.tpl /var/local/dockprox/upstream.conf.tpl
COPY ./server.conf.tpl /var/local/dockprox/server.conf.tpl
COPY ./dockprox.cron /etc/cron.d/dockprox.cron
RUN /usr/bin/crontab /etc/cron.d/dockprox.cron
#RUN /bin/systemctl enable cron 
#RUN service cron start
#CMD ["/etc/init.d/cron","start"]

#docker build -t nginx-dockprox .
#docker run --restart unless-stopped --name nginx-dockprox -p 80:80 -v /var/run/docker.sock:/var/run/docker.sock:ro -d nginx-dockprox
