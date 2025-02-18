# Part 1. Ready-made docker

Взял официальный докер-образ с nginx и выкачал его при помощи ```docker pull```

![Alt text](src/screenshots/p1.1.png)

Проверил наличие докер-образа через ```docker images```

![Alt text](src/screenshots/p1.2.png)

Запустил докер-образ через ```docker run -d [image_id|repository]```

Проверил, что образ запустился через ```docker ps```

![Alt text](src/screenshots/p1.3.png)

Посмотрел информацию о контейнере через ```docker inspect [container_id|container_name]```

По выводу команды определилил и поместил в отчёт размер контейнера, список замапленных портов и ip контейнера.

![Alt text](src/screenshots/p1.4.png)

Остановил докер контейнер через ```docker stop [container_id|container_name]```

Проверил, что контейнер остановился через docker ps.

![Alt text](src/screenshots/p1.5.png)

Запустил докер с портами 80 и 443 в контейнере, замапленными на такие же порты на локальной машине, через команду ```run```

![Alt text](src/screenshots/p1.6.png)

Проверил, что в браузере по адресу localhost:80 доступна стартовая страница nginx.

![Alt text](src/screenshots/p1.7.png)

Перезапустил докер контейнер через ```docker restart [container_id|container_name]```

![Alt text](src/screenshots/p1.8.png)

# Part 2. Operations with container

Прочитал конфигурационный файл nginx.conf внутри докер контейнера через команду exec.

![Alt text](src/screenshots/p2.1.png)


Создал на локальной машине файл nginx.conf.
Настроил в нем по пути /status отдачу страницы статуса сервера nginx.

![Alt text](src/screenshots/p2.2.png)

Скопировал созданный файл nginx.conf внутрь докер-образа через команду ```docker cp```

Перезапустил nginx внутри докер-образа через команду exec.

Проверил, что по адресу localhost:80/status отдается страничка со статусом сервера nginx.

![Alt text](src/screenshots/p2.3.png)

Экспортировал контейнер в файл container.tar через команду ```export```

![Alt text](src/screenshots/p2.4.png)

Остановил контейнер.

![Alt text](src/screenshots/p2.5.png)

Удалил образ через docker rmi [image_id|repository], не удаляя перед этим контейнеры.

Удалил остановленный контейнер.

![Alt text](src/screenshots/p2.6.png)

Импортировал контейнер обратно через команду ```import```

Так же добавил параметр -c, --change. 

  -Применить инструкцию Dockerfile к созданному образу

Так выглядит команда целиком ```docker import --change 'CMD ["nginx", "-g","daemon off;"]' container.tar icmp:latest```  
Здесь я указал команду по умолчанию (CMD), которая будет выполняться при запуске контейнера. В данном случае:
nginx: это команда для запуска веб-сервера Nginx.
-g 'daemon off;': этот флаг указывает Nginx работать в переднем плане (foreground), а не запускаться как демон. Это важно для работы контейнеров Docker, так как процесс должен оставаться активным в переднем плане, чтобы контейнер не завершился.

![Alt text](src/screenshots/p2.7.png)

Проверил, что по адресу localhost:80/status отдается страничка со статусом сервера nginx.

# Part 3. Mini web server

Запустил контейнер nginx с именем  fcgi_serv и портами 80:80 443:433 81:81

![Alt text](src/screenshots/p3.1.png)

Написал мини-сервер на C и FastCgi, который будет возвращать простейшую страничку с надписью Hello World!.

![Alt text](src/screenshots/p3.2.png)

Написал свой nginx.conf, который будет проксировать все запросы с 81 порта на 127.0.0.1:8080.

![Alt text](src/screenshots/p3.3.png)

Теперь копируем nginx.conf и hello_world.c в контейнер /etc/nginx/

```docker cp /Users/karleenr/project/DO5_SimpleDocker-1/src/p3_nginx_conf/nginx.conf fcgi_serv:/etc/nginx/nginx.confz```

```docker cp /Users/karleenr/project/DO5_SimpleDocker-1/src/p3_nginx_conf/hello_world.c fcgi_serv:/etc/nginx/```

![Alt text](src/screenshots/p3.4.png)

Заходим в контейнер 

```docker exec -it $(docker ps -q) sh```

![Alt text](src/screenshots/p3.5.png) 

$(docker ps -q) выводит только ID контейнера 

Обновляем и загружаем все необходимые инструменты для работы с FCGI и С.

```apt-get update && apt-get install -y gcc g++ make libfcgi-dev spawn-fcgi pkg-config```

![Alt text](src/screenshots/p3.6.png)

Компилируем наш .c файл

```gcc hello_world.c -o hello_world.fcg `pkg-config --cflags --libs fcgi```

![Alt text](src/screenshots/p3.7.png)

После запускаем наш исходник с помощью spawn-fcgi на порту 8080.

```spawn-fcgi -n -p 8080 hello_world.fcg```

![Alt text](src/screenshots/p3.8.png)

Выходим из контейнера командой ```exit``` и обновляем его

![Alt text](src/screenshots/p3.9.png)

Проверил, что в браузере по localhost:81 отдается написанная тобой страничка.

![Alt text](src/screenshots/p3.10.png)

# Part 4. Your own docker

Написал свой докер-образ, который:

1) собирает исходники мини сервера на FastCgi из Части 3;

2) запускает его на 8080 порту;

3) копирует внутрь образа написанный ./nginx/nginx.conf;

4) запускает nginx.

![Alt text](src/screenshots/p4.1.png)

![Alt text](src/screenshots/p4.2.png)

Соберал написанный докер-образ через docker build при этом указав имя и тег.

Проверил через docker images, что все собралось корректно.

Запустил собранный докер-образ с маппингом 81 порта на 80 на локальной машине и маппингом папки ./nginx внутрь контейнера по адресу, где лежат конфигурационные файлы nginx'а

Проверил, что по localhost:80 доступна страничка написанного мини сервера.

![Alt text](src/screenshots/p4.3.png)

Дописал в ./nginx/nginx.conf проксирование странички /status, по которой надо отдавать статус сервера nginx.

![Alt text](src/screenshots/p4.5.png)

Перезапустил докер-образ и проверил, что теперь по localhost:80/status отдается страничка со статусом nginx

![Alt text](src/screenshots/p4.4.png)

# Part 5. Dockle

Установил Dokcle командой ```brew install goodwithtech/r/dockle```

После установки Docker нужно проверить докер-обрал командой ```dockle [image_id|repository]```. Но перед этим нужно запушить образ в Докер Хаб, тк локально Dockel образа не видит. 

```docker push karleenr/p4_image:1.0```

![Alt text](src/screenshots/p5.1.png)

Командой ```dockle karleenr/p4_image:1.0``` проверил образ на наличие уязвимостей

![Alt text](src/screenshots/p5.2.png)

## Ошибки: 

```FATAL	- CIS-DI-0010:``` относится к рекомендациям CIS (Center for Internet Security) по безопасности контейнеров. Эта ошибка указывает на то, что в Dockerfile или образе контейнера обнаружены потенциальные проблемы с безопасностью, связанные с хранением учетных данных или конфиденциальной информации в переменных окружения или файлах.


```FATAL - DKL-DI-0005:``` Clear apt-get caches указывает на то, что в Dockerfile необходимо очищать кэш APT после установки пакетов. Это является рекомендацией по безопасности и оптимизации размера образа.


```WARN - CIS-DI-0001:``` Create a user for the container указывает на то, что в Dockerfile контейнер по умолчанию запускается от имени пользователя root, что считается небезопасным. Рекомендуется создавать отдельного пользователя для выполнения приложений внутри контейнера, чтобы минимизировать риски безопасности. 


## Исправление ошибок:

```FATAL - DKL-DI-0005:``` добавил в Dockerfile очистку кэша после установки пакетов

```WARN - CIS-DI-0001:``` создал нового пользователя без пароля

```FATAL	- CIS-DI-0010:``` при запуске dockle указываю, что разрешаю использование конкретных ключей для работы nginx. ```--ak NGINX_GPGKEY --ak NGINX_GPGKEY_PATH --ak NGINX_GPGKEYS```

Так выглядит исправленный Dockerfile

![Alt text](src/screenshots/p5.3.png)

Проверка на ошибки и предупреждения

![Alt text](src/screenshots/p5.4.png)

Остались только информационные сообщения от Dockle

```INFO	- CIS-DI-0005:``` Enable Content trust for Docker
	* export DOCKER_CONTENT_TRUST=1 before docker pull/build

```INFO	- CIS-DI-0006:``` Add HEALTHCHECK instruction to the container image
	* not found HEALTHCHECK statement

```INFO	- CIS-DI-0008:``` Confirm safety of setuid/setgid files

# Part 6. Basic Docker Compose

Напишу файл docker-compose.yml, с помощью которого:

1) Подниму докер-контейнер из Part 5.

2) Подниму докер-контейнер с nginx, который будет проксировать все запросы с 8080 порта на 81 порт первого контейнера.

Так же замаплю 8080 порт второго контейнера на 80 порт локальной машины.

Так выглядит docker-compose.yml

![Alt text](src/screenshots/p6.1.png)

Так же напишу nginx.conf который слушает порт 8080 и проксирует первый контейнер (local_cont) на порт 81

![Alt text](src/screenshots/p6.2.png)

Остановил все запущенные контейнеры.

Соберал и запустил проект с помощью команд ```docker-compose build``` и ```docker-compose up``` или их комбинация (docker-compose up --build). Запускать нужно из директории, в которой находится docker-compose.

![Alt text](src/screenshots/p6.3.png)

Так же в терминале вывлдятся логи с контейнеров

![Alt text](src/screenshots/p6.4.png)