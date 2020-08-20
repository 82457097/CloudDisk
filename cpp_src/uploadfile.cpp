#include"uplaodfile.h"

using namespace std;

bool Upload::AcceptFile() {
	while (fastCGI.FcgiAccept()) {
		//获取环境变量CONTENT_LENGTH的值
		fastCGI.contentLen = getenv("CONTENT_LENGTH");
		//cout << "Content-type: text/html\r\n\r\n" << endl;

		//将环境变量CONTENT_LENGTH的值转化为整数
		if (fastCGI.contentLen != NULL) {
			buflen = strtol(fastCGI.contentLen, nullptr, 10);
		}

		if (buflen <= 0) {
			cout << "No data from standard input.<p>\n" << endl;
		}
		else {
			int tmpch;
			fileData = (char*)malloc(buflen);
			pbegin = pend = ptemp;

			for (int i = 0; i < buflen; ++i) {
				if ((tmpch = getchar()) < 0) {
					cout << "Error: Not enough bytes received on standard input<p>\n" << endl;
					break;
				}
				*ptemp = tmpch;
				++ptemp;
			}

			pend = ptemp;
		}

		ParseDataAndSave();
		UploadFile(fileName, fileName);
		SaveToMysql();
	}

	return true;
}

bool Upload::ParseDataAndSave() {
	ptemp = strstr(pbegin, "\r\n");
	strncpy(fastCGI.boundary, pbegin, ptemp - pbegin);

	ptemp += 2;
	buflen -= (ptemp - pbegin);
	pbegin = ptemp;
	char* pfileNameBegin = strstr(pbegin, "filename=");
	pfileNameBegin += strlen("filename=");  /* 指向第一个双引号 */
	char* pfileNameEnd = strchr(pfileNameBegin, '"');  /* 指向第二个双引号 */
	strncpy(fileName, ++pfileNameBegin, pfileNameEnd - pfileNameBegin);
	cout << "<br>filename: %s<br>" << endl;

	/* 第三行、四行 */
	ptemp = strstr(pbegin, "\r\n");
	ptemp += 2;
	buflen -= (ptemp - pbegin);  /* 剩余长度 */
	pbegin = ptemp;
	ptemp = strstr(pbegin, "\r\n");
	ptemp += 4;
	buflen -= (ptemp - pbegin);  /* 所需数据长度 */

	/* 开始正文 */
	pbegin = ptemp;

	int boderLen = strlen(fastCGI.boundary);
	int FullDataLen = buflen - boderLen + 1;
	for (int i = 0; i < FullDataLen; ++i) {
		if (*pbegin == *fastCGI.boundary) {
			if (memcmp(pbegin, fastCGI.boundary, boderLen) == 0) {
				break;
			}
		}
		++pbegin;
	}
	swap(pbegin, ptemp);
	if (ptemp == nullptr) {
		ptemp = pend;
	}

	ptemp -= 2;  /* 退两个字符：/r/n */

	/* 本地保存 */
	int fd = open(fileName, O_CREAT | O_WRONLY, 0664);
	write(fd, pbegin, ptemp - pbegin);
	close(fd);
	
	return true;
}

bool Upload::UploadFile(char *fileName, char *fileId) {
	if (!fastDFS.FdfsClientInit()) {
		//cout << "fdfs_client初始化失败！" << endl;
		return false;
	}

	if (!fastDFS.TrackerGetConnection()) {
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.TrackerQueryStorageStore()) {
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.StorageUploadByFilename1(fileName, fileId)) {
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.TrackerCloseConnectionEx()) {
		fastDFS.FdfsClientDestroy();
		return false;
	}

	fastDFS.FdfsClientDestroy();

	return true;
}

bool Upload::SaveToMysql() {
	//mysql.MysqlConnect();
	char sql[SQL_LEN] = { "\0" };
	snprintf(sql, SQL_LEN, "insert into %s values(NULL, '%s', '%s')", TABLE_NAME, fileName, fileId);
	int flag = mysql.MysqlQuery(sql);
	if (flag == 0) {
		return false;
	}

	return true;
}