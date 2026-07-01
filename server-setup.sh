#!/bin/sh

sudo cp -fv  run.cgi    /usr/lib/cgi-bin/run.cgi
sudo cp -fv  index.html /var/www/html/index.html
sudo cp -rfv ftext/     /opt
sudo make -C /opt/ftext

sudo systemctl restart apache2
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
