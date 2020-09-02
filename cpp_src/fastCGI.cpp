#include"fastCGI.h"
#include<iostream>

using namespace std;

bool FastCGI::FcgiAccept() {
	if (FCGI_Accept() < 0) {
		LOG("FCGI_Accept start failed.");
		return false;
	}
	else {
		LOG("FCGI_Accept successful.");
		return true;
	}

	return false;
}