#!/bin/bash

select choice in normal_start debug_start
do
	case "$choice" in
		normal_start)
			echo "normal startup..."
			spawn-fcgi -a 127.0.0.1 -p 10003 -f ./upload_cpp.cgi
		break
		;;
		debug_start)
			echo "debug startup..."
			spawn-fcgi -a 127.0.0.1 -p 10003 -f ./upload_cpp.cgi -n
		break
		;;
	esac
done
