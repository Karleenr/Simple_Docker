FROM nginx

COPY nginx.conf /etc/nginx/nginx.conf

COPY hello_world.c /etc/nginx/

WORKDIR /etc/nginx/

RUN apt-get update && \
    apt-get install -y iproute2  gcc g++ make libfcgi-dev spawn-fcgi pkg-config

RUN gcc hello_world.c -o hello_world.fcg `pkg-config --cflags --libs fcgi`

CMD spawn-fcgi -p 8080 hello_world.fcg && nginx -g 'daemon off;'

