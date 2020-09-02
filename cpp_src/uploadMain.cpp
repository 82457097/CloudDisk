#include"uplaodfile.h"

int main() {
	Upload upload;
	if (!upload.mysql.MysqlInit()) {
		LOG("Init failed!");
		return -1;
	}
	if (upload.AcceptFile()) {
		LOG("Accept files failed!");
		return -1;
	}
	if (upload.mysql.MysqlDisconnect()) {
		LOG("Failed to disconnect mysql connect!");
		return -1;
	}

	return 0;
}