ROADMAP / KNOWN ISSUES
======================
* integrar nginx
* ver si usamos el template del odoo.conf y reemplazamos valores ya sea iniciando odoo y bajandolo, usando sed o usando script que generamos en python
* ver si queremos descargar y exponer todos los sources descargados a un path local para facilitar desarrollo. Por un lado esta este issue (https://github.com/docker/docker/issues/3639) que si montamos en un dockerfile luego no podemos agregar en otros más adelante. POdriamos usar docker cp para copiar todo lo que está en el container al host
PROBAR shell!!!

docker-compose:
* Ver si sirve:
    * container_name
    * hostname
    * restart: always
    * dns
* diferencia depends_on o link para db
