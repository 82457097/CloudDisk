#include"uplaodfile.h"

int main() {
	Upload upload;
	if (!upload.mysql.MysqlInit()) {
		cout << "Init failed!" << endl;
		return -1;
	}
	if (upload.AcceptFile()) {
		cout << "Accept files failed!" << endl;
		return -1;
	}
	if (upload.mysql.MysqlDisconnect()) {
		cout << "Failed to disconnect mysql connect!" << endl;
		return -1;
	}

	return 0;
}