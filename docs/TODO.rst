ROADMAP / KNOWN ISSUES
======================
* Certificados para afip, certificados ssl, certificado para infra conectarse, certificado para descarga repo
* Otras cosas: ver de agregar scripts para agilizar desarrollo como por ejemplo: montado de carpeta, hacer execute a server, 
* ver si para desarrollo es utili usar una build local que permita hacer apt-get update, instalar ssh y agregar otras dependencias
* backups y sync a google drive
* agregar mas parametros al conf "timezone = Europe/Paris", idioma, country, proxy mode, log, etc
* ver manera practica de usar compose para version 8 y 9 indistintamente (y/o distintos productos)
* implementar load si sigue sin andar el de odoo que toma del odoo conf
* change aeroo docs to ubuntu 16.04 https://github.com/ingadhoc/docker-aeroo-docs/blob/master/Dockerfile (agregar estas fuentes sudo apt-get install msttcorefonts)
* integrar nginx (implementar con -p para levantar varios proyectos)
* integrar distintos ambientes de test, prod etc
* ver si usamos el template del odoo.conf y reemplazamos valores ya sea iniciando odoo y bajandolo, usando sed o usando script que generamos en python
* ver si queremos descargar y exponer todos los sources descargados a un path local para facilitar desarrollo. Por un lado esta este issue (https://github.com/docker/docker/issues/3639) que si montamos en un dockerfile luego no podemos agregar en otros más adelante. POdriamos usar docker cp para copiar todo lo que está en el container al host
* descargar repositorios privados usando claves
* terminar de entender los volumes y ver si es necesario definirlos en el compose o no

docker-compose:
* Ver si sirve:
    * container_name
    * hostname
    * restart: always
    * dns
* diferencia depends_on o link para db
