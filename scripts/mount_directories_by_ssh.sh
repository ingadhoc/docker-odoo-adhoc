#!/bin/bash
mkdir data/odoo
sshfs -o identityFile=~/.ssh/id_rsa root@172.27.0.3:/opt/odoo/ ./data/odoo