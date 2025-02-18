FROM nginx

RUN apt-get update && apt-get install -y \
    gcc \
    build-essential \
    spawn-fcgi \
    libfcgi-dev \
    iproute2 \
    g++ \
    pkg-config \
    && useradd -m shrek \
    && rm -rf /var/lib/apt/lists/* 

COPY hello_world.c /etc/nginx/

COPY nginx.conf /etc/nginx/nginx.conf  

WORKDIR /etc/nginx/  

RUN gcc -o hello_world hello_world.c -lfcgi `pkg-config --cflags --libs fcgi` && \ 
    touch /var/run/nginx.pid && \
    chown -R shrek:shrek /etc/nginx /var/cache/nginx /var/run/nginx.pid && \
    mkdir -p /var/cache/nginx/client_temp

USER shrek

CMD service nginx start && spawn-fcgi -p 8080 -n /etc/nginx/hello_world && nginx -s reload


