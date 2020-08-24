#pragma once
#include <fcntl.h>
#include <unistd.h>
#include <fcgi_config.h>
#include <fcgi_stdio.h>


#define BOUNDARY_LEN 256

class FastCGI {
public:
	char *contentLen;
	char boundary[BOUNDARY_LEN] = { '\0' };
public:
	FastCGI() {}
	~FastCGI() {}
	bool FcgiAccept();
};
