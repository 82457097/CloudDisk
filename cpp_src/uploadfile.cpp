#include"uplaodfile.h"

using namespace std;

bool Upload::AcceptFile() {
	while (fastCGI.FcgiAccept()) {
		//��ȡ��������CONTENT_LENGTH��ֵ
		fastCGI.contentLen = getenv("CONTENT_LENGTH");
		//cout << "Content-type: text/html\r\n\r\n" << endl;

		//����������CONTENT_LENGTH��ֵת��Ϊ����
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
	pfileNameBegin += strlen("filename=");  /* ָ���һ��˫���� */
	char* pfileNameEnd = strchr(pfileNameBegin, '"');  /* ָ��ڶ���˫���� */
	strncpy(fileName, ++pfileNameBegin, pfileNameEnd - pfileNameBegin);
	cout << "<br>filename: %s<br>" << endl;

	/* �����С����� */
	ptemp = strstr(pbegin, "\r\n");
	ptemp += 2;
	buflen -= (ptemp - pbegin);  /* ʣ�೤�� */
	pbegin = ptemp;
	ptemp = strstr(pbegin, "\r\n");
	ptemp += 4;
	buflen -= (ptemp - pbegin);  /* �������ݳ��� */

	/* ��ʼ���� */
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

	ptemp -= 2;  /* �������ַ���/r/n */

	/* ���ر��� */
	int fd = open(fileName, O_CREAT | O_WRONLY, 0664);
	write(fd, pbegin, ptemp - pbegin);
	close(fd);
	
	return true;
}

bool Upload::UploadFile(char *fileName, char *fileId) {
	if (!fastDFS.FdfsClientInit()) {
		//cout << "fdfs_client��ʼ��ʧ�ܣ�" << endl;
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