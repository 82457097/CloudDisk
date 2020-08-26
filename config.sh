#!/bin/bash

NGINX_PATH=/usr/local/nginx

if [ ! -d ${NGINX_PATH}/work ]; then
	mkdir ${NGINX_PATH}/work
else
	echo "work/ is exsists."
fi

if [ ! -d ${NGINX_PATH}/work/zyFile2 ]; then
	cp -rf ./html/zyFile2 ${NGINX_PATH}/work
else
	echo "zyFile2 is exsists."
fi


#sed -i '/\<server_name/a\location / {\n    root   work/zyFile2;\n    index  demo.html;\n}\n\nlocation /upload {\n    fastcgi_pass 127.0.0.1:10003;\n    include fastcgi.conf;\n}' \
#	${NGINX_PATH}/conf/nginx.conf
