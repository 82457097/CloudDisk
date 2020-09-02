#include"uplaodfile.h"
#include<sys/mman.h>
#include<sys/types.h>
#include<unistd.h>

using namespace std;

bool Upload::AcceptFile() {
	while (fastCGI.FcgiAccept()) {
		fastCGI.contentLen = getenv("CONTENT_LENGTH");
		printf("Content-type: text/html\r\n\r\n");

		if (fastCGI.contentLen != nullptr) {
			buflen = strtol(fastCGI.contentLen, nullptr, 10);
		}

		if (buflen <= 0) {
			LOG("No data from standard input.");
		}
		else {
			LOG("Recieve size is %d", buflen);
			char tmpch;
			fileData = (char*)malloc(buflen);
			pbegin = ptemp = fileData;
			
			for (int i = 0; i < buflen; ++i) {
				tmpch = getchar();
				*ptemp = tmpch;
				++ptemp;
			}
			
			pend = ptemp;
		}

		ParseDataAndSave();
		UploadFile(fileName);
		SaveToMysql();
		free(fileData);
		unlink(fileName);
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
	pfileNameBegin += strlen("filename=");  
	char* pfileNameEnd = strchr(++pfileNameBegin, '"');  
	strncpy(fileName, pfileNameBegin, pfileNameEnd - pfileNameBegin);
	printf("<br>filename: %s<br>\n", fileName);

	ptemp = strstr(pbegin, "\r\n");
	ptemp += 2;
	buflen -= (ptemp - pbegin);  
	pbegin = ptemp;
	ptemp = strstr(pbegin, "\r\n");
	ptemp += 4;
	buflen -= (ptemp - pbegin);  


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

	ptemp -= 2;  

	int fileLen = ptemp - pbegin;
	int fd = open(fileName, O_CREAT | O_RDWR, 0664);
	if(fd < 0) {
		LOG("open file failed");
	}
	if(lseek(fd, fileLen - 1, SEEK_SET) == -1) {
		LOG("lseek set failed.");
	}
	write(fd, " ", 1);
	
	void *mptr = mmap(NULL, fileLen, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(mptr == MAP_FAILED) {
		LOG("Map failed.");
	}

	memcpy(mptr, pbegin, fileLen);
	
	close(fd);
	munmap(mptr, fileLen);
	
	return true;
}

bool Upload::UploadFile(char* fileName) {
	if (!fastDFS.FdfsClientInit()) {
		LOG("fastdfs initial failed.");
		return false;
	}

	if (!fastDFS.TrackerGetConnection()) {
		LOG("tracker initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.TrackerQueryStorageStore()) {
		LOG("storage initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.StorageUploadByFilename1(fileName, fileId)) {
		LOG("StorageUploadByFilename1 initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	if (!fastDFS.TrackerCloseConnectionEx()) {
		LOG("TrackerCloseConnectionEx initial failed.");
		fastDFS.FdfsClientDestroy();
		return false;
	}

	fastDFS.FdfsClientDestroy();
	printf("<br>fileid: %s\n<br>", fileId);
	
	return true;
}

bool Upload::SaveToMysql() {
	char sql[SQL_LEN] = { '\0' };
	snprintf(sql, SQL_LEN, "insert into %s values(NULL, '%s', '%s')", TABLE_NAME, fileName, fileId);
	//cout << sql << endl;
	int flag = mysql.MysqlQuery(sql);
	if (flag == 0) {
		return true;
	}

	return false;
}
