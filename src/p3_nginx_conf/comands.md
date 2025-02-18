docker run -p 80:80 -p 443:443 -p 81:81 --name fcgi_serv nginx

docker cp /Users/karleenr/project/DO5_SimpleDocker-1/src/p3_nginx_conf/nginx.conf fcgi_serv:/etc/nginx/nginx.conf
docker cp /Users/karleenr/project/DO5_SimpleDocker-1/src/p3_nginx_conf/hello_world.c fcgi_serv:/etc/nginx/
docker exec -it $(docker ps -q) sh //ошибка мог не зоходить в интерактивный терминал, а просто указывать где и что запустить))
cd /etc/nginx 
apt-get update && apt-get install -y gcc g++ make libfcgi-dev spawn-fcgi pkg-config
gcc hello_world.c -o hello_world.fcg `pkg-config --cflags --libs fcgi`
spawn-fcgi -n -p 8080 hello_world.fcg
exit
docker exec -it $(docker ps -q) nginx -s reload