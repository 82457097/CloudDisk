#pragma once

#define BOUNDARY_LEN 256

class FastCGI {
public:
	char *contentLen;
	char boundary[BOUNDARY_LEN] = { "\0" };
public:
	FastCGI() {}
	~FastCGI() {}
	bool FcgiAccept();
};