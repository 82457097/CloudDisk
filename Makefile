
upload.cgi:
	gcc -o bin/uploadMain.cgi src/uploadMain.c src/upload.c src/sqlapi.c src/log.c -Iinclude -I/usr/include/fastdfs/ -I/usr/include/fastcommon/ -L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi  -std=c99 -D_SVID_SOURCE

showFiles.cgi:
	gcc -o bin/showFiles.cgi src/showFiles.c src/sqlapi.c src/log.c -Iinclude -I/usr/include/fastdfs/ -I/usr/include/fastcommon/ -L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi  -std=c99 -D_SVID_SOURCE

upload_cpp.cgi:
	g++ -o bin/upload_cpp.cgi cpp_src/*.cpp cpp_src/*.h -I/usr/include/fastdfs/ -I/usr/include/fastcommon/ -L/usr/lib64 -lmysqlclient -lpthread -lfdfsclient -lfcgi -std=c++11 -D_SVID_SOURCE
