FROM debian:trixie

RUN apt-get update && \
    apt-get install -y \
        apache2 \
        libcgi-pm-perl \
        gcc \
        make && \
    rm -rf /var/lib/apt/lists/*

RUN a2enmod cgid

COPY ./index.html  /var/www/html/index.html
COPY ./run.cgi     /usr/lib/cgi-bin/run.cgi
COPY ./ftext       /opt/ftext

RUN make -C /opt/ftext

EXPOSE 80

CMD ["apachectl", "-D", "FOREGROUND"]
