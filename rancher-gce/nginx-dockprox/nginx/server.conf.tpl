server {
    listen 80;
    server_name {{cPublicServerName}};

    # proxy header and settings
    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-Host $host;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_redirect off;

    # odoo log files
    access_log /var/log/nginx/access_{{cUpstreamServerName}};
    error_log  /var/log/nginx/error_{{cUpstreamServerName}};

    # increase proxy buffer size
    proxy_buffers 16 64k;
    proxy_buffer_size 128k;

    # force timeouts if the backend dies
    proxy_next_upstream error timeout invalid_header http_500 http_502 http_503;

    # enable data compression
    gzip on;
    gzip_min_length 1100;
    gzip_buffers 4 32k;
    gzip_types text/plain application/x-javascript text/xml text/css;
    gzip_vary on;

    # cache some static data in memory for 60mins.
    # under heavy load this should relieve stress on the OpenERP web interface a bit.
    location ~* /web/static/ {
        proxy_cache_valid 200 60m;
        proxy_buffering    on;
        expires 864000;
        proxy_pass http://{{cUpstreamServerName}};
    }

    location / {
        proxy_pass http://{{cUpstreamServerName}};
    }

    location /longpolling {
        proxy_pass http://{{cUpstreamServerNameChat}};
    }
}
