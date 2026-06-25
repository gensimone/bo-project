FROM debian:bookworm

RUN apt-get update && \
    apt-get install -y \
        apache2 \
        libcgi-pm-perl \
        gcc \
        gdb \
        vim \
        python3 \
        make && \
    rm -rf /var/lib/apt/lists/*

RUN a2enmod cgid

RUN mkdir /cores
RUN chmod 777 /cores
RUN echo "set -o vi" >> /root/.bashrc

COPY ./payload-gen.py /root/payload-gen.py
COPY ./simple-vuln.c  /root/simple-vuln.c
COPY ./index.html     /var/www/html/index.html
COPY ./run.cgi        /usr/lib/cgi-bin/run.cgi
COPY ./ftext          /opt/ftext

RUN gcc -g -O0 -fno-stack-protector -z execstack -fno-pic -no-pie /root/simple-vuln.c -o /root/simple-vuln
RUN make clean -C /opt/ftext
RUN make -C /opt/ftext

EXPOSE 80

CMD ["apachectl", "-D", "FOREGROUND"]
