#!/bin/sh

# Uncomment this to enable the web server presentation.
# sudo cp -fv  webserver/run.cgi    /usr/lib/cgi-bin/run.cgi
# sudo cp -fv  webserver/index.html /var/www/html/index.html
# sudo cp -rfv ftext/     /opt
# sudo make -C /opt/ftext
# sudo systemctl restart apache2

# Disable ASLR (Address Space Layout Randomization).
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
