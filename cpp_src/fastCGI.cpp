#include"fastCGI.h"

bool FastCGI::FcgiAccept() {
	if (!FCGI_Accept()) {
		return false;
	}

	return true;
}