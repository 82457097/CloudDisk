#!/bin/bash

Green1="\033[32m"
Green2="\033[0m"

GET_NGINX=http://nginx.org/download/nginx-1.17.8.tar.gz
GET_LIBCOMMOM=https://github.com/happyfish100/libfastcommon/archive/master.zip
GET_FASTDFS=https://github.com/happyfish100/fastdfs/archive/master.zip
GET_FNM=https://github.com/happyfish100/fastdfs-nginx-module/archive/master.zip
NGINX_POS=/usr/local/nginx/

OsCheck() {
	if [ -e /etc/redhat-release ]; then
		REDHAT=`cat /etc/redhat-release | awk '{print $1}'`
	else
		DEBIAN=`cat /etc/issue | awk '{print $1}'`
	fi

	if [ "$REDHAT" == "CentOS" -o "$REDHAT" == "Red" ]; then
		P_M=yum
	elif [ "$DEBIAN" == "Ubuntu" -o "$DEBIAN" == "ubuntu" ]; then
		P_M=apt-get
	else
		Operating system dose not support.
		exit 1
	fi
	
	if [ "$P_M" == "yum" ]; then
		echo -e "${Green1}Your system is $REDHAT. $Green2"
	else
		echo -e "${Green1}Your system is $DEBIAN. $Green2"
	fi
}

OsCheck
echo "-----------------------------------------------"

InstallNginx() {
	NGINX_PAC=nginx-1.17.8.tar.gz

	echo "Download nginx..."
	if [ ! -e ./$NGINX_PAC ]; then
		wget $GET_NGINX
	else
		echo -e "${Green1}Nginx has been download. $Green2"
	fi

	echo "Install pcre zlib openssl..."
	sudo $P_M install libpcre3 libpcre3-dev zlib1g-dev openssl libssl-dev
	
	echo "Install nginx..."
	if [ ! -e ./$NGINX_PAC ]; then
		echo "Can't find nginx package..."
	elif [ ! -d ./nginx-1.17.8/ ]; then
		tar -xvzf $NGINX_PAC
	else
		cd nginx-1.17.8
		./configure
		make
		make install
		cd -
	fi
	
	echo "Link nginx..."
	sudo ln -s ${NGINX_POS}sbin/nginx /usr/local/bin
	echo -e "${Green1}Install nginx Success! $Green2"
}

if [ -d $NGINX_POS ]; then
	echo -e "${Green1}Nginx has been Installed. $Green2"
	echo "----------------------------------------------"
else
	echo "----------------------------------------------"
	InstallNginx
fi

Install_libfastcommon() {
	LFC_PAC=libfastcommon-master.zip
	LFC_DIR=libfastcommon-master
	echo "----------------------------------------------"
	echo "Download FastDFS..."
	echo "Install libfastcommon..."
	if [ ! -d ./$LFC_DIR/ -a ! -e master.zip ]; then
		wget $GET_LIBCOMMOM
		unzip master.zip
		mv master.zip $LFC_PAC
	else
		echo -e "${Green1}Libfastcommon has been download. $Green2"
	fi

	if [ -e ./$LFC_DIR/ ]; then
		cd $LFC_DIR/
		./make.sh clean
		./make.sh
		./make.sh install
		cd -
	else
		echo "Don't have libfastcommon files!"
	fi
		
	echo -e "${Green1}Libfastcommon install success. $Green2"
}
if [ ! -e /usr/lib64/libfastcommon.so ]; then
	Install_libfastcommon
else
	echo -e "${Green1}Libfastcommon has been installed.$Green2"
	echo "-----------------------------------------------"
fi

Install_fastdfs() {
	FD_PAC=fastdfs-master.zip
	FD_DIR=fastdfs-master
	echo "Install fastDFS..."
	if [ ! -e ./master.zip -a ! -d ./$FD_DIR/ ]; then
		wget $GET_FASTDFS
		unzip master.zip
		mv master.zip $FD_PAC
	else
		echo -e "${Green1}Fastdfs has been download.$Green2" 	
	fi
	
	if [ -d ./$FD_DIR/ ]; then
		cd $FD_DIR
		./make.sh clean
		./make.sh
		sudo ./make.sh install
		cd -
	else
		echo "Don't have fastDFS files!"
	fi

	if [ -d /etc/fdfs/ ]; then
		cd /etc/fdfs
		cp tracker.conf.sample tracker.conf
		cp storage.conf.sample storage.conf
		cp client.conf.sample client.conf
	fi
	
	echo -e "${Green1}FastDFS install success! $Green2"
}

if [ ! -e /usr/bin/fdfs_monitor ]; then
	Install_fastdfs
else
	echo -e "${Green1}FastDFS has been installed.$Green2"
fi

Install_fastdfs_nginx_module() {
	FNM_DIR=fastdfs-nginx-module-master
	FNM_PAC=fastdfs_nginx_module.zip

	echo "Install fastdfs_nginx_module..."
	if [ ! -e ./master.zip -a ! -d ./${FNM_DIR}/ ]; then
		wget $GET_FNM
		unzip master.zip
		mv master.zip $FNM_PAC
	else
		echo -e "${Green1}Fastdfs_nginx_module has been download."
	fi

	echo -e "${Green1}Fastdfs_nginx_module download success."
}

if [ ! -d ./${FNM_DIR}/ ]; then
	echo "-------------------------------------------------------"
	Install_fastdfs_nginx_module
else
	echo "-------------------------------------------------------"
	echo -e "${Green1}Fastdfs_nginx_module has been download.$Green2"
fi

Modify_config() {
	FNM_DIR=fastdfs-nginx-module-master

	if [ -e ./$FNM_DIR/src/config ]; then
		sed -i '/\<ngx_module_incs/'d $FNM_DIR/src/config
		sed -i '/\<CORE_INCS/'d	$FNM_DIR/src/config
		sed -i '/\<ngx_module_libs/a\ngx_module_incs="/usr/include/fastdfs /usr/include/fastcommon/"' $FNM_DIR/src/config
		sed -i '/\<NGX_ADDON_SRCS/a\CORE_INCS="$CORE_INCS /usr/include/fastdfs /usr/include/fastcommon/"' $FNM_DIR/src/config
	else
		echo "Fastdfs_nginx_module hasn't download."
	fi
}

string=`cat fastdfs-nginx-module-master/src/config | grep "/usr/include/fastcommon/" | awk 'NR==1{print $NF}'`
if [ "$string" != "/usr/include/fastcommon/\"" ]; then
	Modify_config
	echo -e "${Green1}Config modify success.$Green2"
else
	echo -e "${Green1}Config has been modified.$Green2"
fi

Add_fnm_to_nginx() {
	CURPOS=`pwd`
#	cd nginx-1.17.8/
#	./configure --add-module=$CURPOS/fastdfs-nginx-module-master/src
#	make
#	make install
	cd $NGINX_POS/sbin
	./nginx -V
	cd -
	cd fastdfs-nginx-module-master/src/
	cp mod_fastdfs.conf /etc/fdfs/
	cd -
	cd fastdfs-master/conf
	cp mime.types http.conf /etc/fdfs/
	cd -
}

Add_fnm_to_nginx












