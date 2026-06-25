#!/bin/sh

sudo cp -f  run.cgi    /usr/lib/cgi-bin/run.cgi
sudo cp -f  index.html /var/www/html/index.html
sudo cp -rf ftext/     /opt

systemctl restart apache2
