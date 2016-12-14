FROM nginx
COPY nginx.conf /etc/nginx/nginx.conf
COPY cert.pem /etc/nginx/cert.pem
COPY cert.key /etc/nginx/cert.key
COPY conf.d/default.conf /etc/nginx/conf.d/default.conf

#docker build -t custom-nginx .
#docker run --restart unless-stopped --name custom-nginx -p 443:443 -d custom-nginx
