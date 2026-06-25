#!/bin/sh

sudo cp -f  run.cgi    /usr/lib/cgi-bin/run.cgi
sudo cp -f  index.html /var/www/html/index.html
sudo cp -rf ftext/     /opt

sudo systemctl restart apache2
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
