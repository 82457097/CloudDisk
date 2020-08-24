#include"fastCGI.h"
#include<iostream>

using namespace std;

bool FastCGI::FcgiAccept() {
	if (FCGI_Accept() < 0) {
		cout << "FCGI_Accept start failed." << endl;
		return false;
	}

	return true;
}