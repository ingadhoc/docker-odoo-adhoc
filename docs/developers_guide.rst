Developers guid and usefull tips
================================
TODO

Use example:
1. Clone this repository with your project name "git clone https://github.com/ingadhoc/docker-odoo-adhoc [project_name]"
2. Enter your project folder "cd [project_name]"
3. Start odoo and create your first database "docker-compose up"
4. Stop your service "ctrl + c"
5. If you want to work in some custom repositories you should:
  a. change permissions to custom_addons folder "sudo chmod a+rw -R data/custom-addons"
  b. clone your repositories into "data/custom-addons"
  c. when you start the server again (docker-compose up) this repositories will be automatically loded to odoo configuration and will have more priority than any repository insde de docker image
6. On development, you usually want to update modules, reinit some and test some odoo command line args. We recommend to run container attached "docker-compose run --rm --service-ports odoo bash"
7. Once attached you can:
  a. run odoo "odoo.py"
  b. run odoo shell
  ::
  odoo.py shell -d [database_name]
