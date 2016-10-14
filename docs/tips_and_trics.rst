Tips and trics
==============
TODO format and finish this

IMPORTANTE
    para esto falta agregar la clave publica!
    (tambien es necesario exponer puerto ssh)
    habria que ver como levantar siempre el container con igual hostname
If you want to access files with an editor outside you can mount a director to yout host by ssh 
    1. Install sshfs "apt-get install sshfs"
    2. Creat local directory to mount ("mkdir data/odoo")
    3. Mount to local directory ("sshfs -o identityFile=~/.ssh/id_rsa root@172.27.0.3:/opt/odoo/ ./data/odoo")
    3. Por ahora, hasta que movamos el odoo server (si instalamos odoo sin runbot) o algo por el estilo, podemos: 
        a: mkdir data/odoo_server
        b. Mount to local directory ("sshfs -o identityFile=~/.ssh/id_rsa root@172.27.0.3:/usr/lib/python2.7/dist-packages/openerp/ ./data/odoo_server")
    4. unmount with "sudo umount -l data/odoo" 
    5. Para hacerlo permanente editamos "sudo nano /etc/fstab" y agregamos al final algo como "sshfs#root@172.27.0.3:/opt/odoo/ /home/jjscarafia/odoo/docker/docker-odoo-adhoc"
    6. luego reiniciamos
    (probar sshfs -o IdentityFile=~/.ssh/id_rsa,auto_cache,reconnect,defer_permissions,negative_vncache,volname=Deploy root@linw.live:/opt/wildfly-8.1.0.Final/standalone/deployments ~/Deploy)

if you want to create different projects you can use
docker-compose up -p [project_name] (default: directory name)

Run services in the background
docker-compose up -d

Stop background containers with docker-compose stop

Create database with some module
docker-compose run --rm --service-ports odoo bash

see what is currently running
docker-compose run --rm odoo -- -d [db_name] -i [modules_list]

Run odoo shell:
docker-compose run odoo -- shell
or odoo.py shell inside container

On development, you usually want to update modules, reinit some and test some odoo command line args. We recommend:
#. Run container attached
::
    docker-compose run --rm --service-ports odoo bash
#. Start odoo and send any arg you want, for eg.
::
    odoo.py -u all

If you want to go inside the container by default odoo user is used but it don't have sudo rights. So, if you want to make thinks with root, we suggest:
#. Run as sudo (publishing ports, disposable container and enter bash)
::
    docker-compose run --rm --service-ports -u root odoo bash
Run odoo with user odoo and preserving env vars
::
    sudo -E -u odoo odoo.y
    (NOTA: en la v9 esto no me anduvo y tuve que entrar a bash y luego correro odoo, sudo -H -E -u odoo bash y luego 'odoo.py' un error de permisos raro)
Run odoo with user odoo and preserving env vars

Copy all included addons to ...


Some eg.:
* docker-compose run --rm --service-ports odoo --workers=0 
* docker-compose run --rm --service-ports odoo copy_sources
* docker-compose run odoo --rm -- --workers=0
* docker-compose run --rm --service-ports odoo bash

# Docker Compose para Odoo

La idea de este docker compose es agilizar y estandarizar la manera en que se levanta odoo. Este compose sirve para odoo v8 y odoo v9 por ahora.

Algunos tips:

* Se debería poder actualizar la info de este repositorio sin problemas con un "git pull"
* Por defecto la versión de odoo utilizada es la 9.0
* la versión de odoo y otros parámetros se pueden configurar en el archivo ".env"
* ese archivo debería ser el ÚNICO que se modifica
* para utilizar docker y docker compose sin "sudo":
    * sudo groupadd docker
    * sudo gpasswd -a $USER docker
    * recomendamos reiniciar linux o ejecutar "sudo service docker restart" y "newgrp docker"

Resumen de los comandos más comunes:

* docker-compose up: arrancar odoo
* docker-compose pull: actualizar nuestro odoo


## Instalación

* instalar docker-compose con "pip install docker-compose"
* instalar docker-engine con "sudo curl -sSL https://get.docker.com/ | sh"
* en algun directorio que deseemos, descargamos este repositorio "git clone git@github.com:ingadhoc/docker-compose-odoo.git"
* levantamos odoo entrando al directorio (cd docker-compose-odoo) y corriendo "docker-compose up"
* accedemos a odoo en http://localhost:8069/


## Agregado de repositorios

La imagen sugerida de odoo ya tiene los repositorios que usamos cargados. Si se quiere trabajar en desarrollo o agregar otros repositorios localmente, entonces:

1. clonar dichos repositorios en la carpeta "sources"
2. editar .env (subl .env) y modificar parámetro "CUSTOM_ADDONS" para que quede de la forma "CUSTOM_ADDONS=/mnt/extra-addons/[nombre_directorio_repositorio]"


## Usando docker compose

* docker-compose up:
    * crea todos los containers y los deja levantados. Crea tambien volumes para la data si no existen ya
    * se puede usar -d para que quede en segundo plano
* docker-compose rm: borra todos los containers pero no los volumes (la data queda persistente)
* docker-compose down -v: borra todos los containers y los volumes (la data no queda persistente)
* docker-compose ps: lista todos los contenedores
* docker-compose pull: pull a imagenes de contenedores (para actualizar nuestras versione más recientes)
* docker-compose run: permite correr un comando sobre un servicio.
    * Por defecto no expone ningun puerto, para hacerlo agregar "--service-ports"
    * Puede ser recomendable agregar --rm cuando queremos tirar un comando y queremos que luego el container se borre
    * ejemplo de uso:
        * crear bd "demo" e instalar "notes" "docker-compose run --rm odoo -d demo -i base"
        * tirar un update de un modulo "docker-compose run --rm odoo -d demo -u base"
* docker-compose config: permite ver la configuracion con la que se esta levantando
* export atributo=valor: permite definir atributos de contexto que el compose toma y se puede ver con el config 
* ver valor de variable "printenv [variable]" (ej. "printenv ADMIN_PASS")
