#pragma once
#include<string>
#include"fastDFS.h"
#include"fastCGI.h"
#include"MySql.h"
#include"logger.h"

using namespace std;


#define FILE_NAME_LEN 512
#define FILE_ID_LEN 512

class File {
public:
	char fileName[FILE_NAME_LEN] = { '\0' };
	char fileId[FILE_ID_LEN] = { '\0' };
	char *fileData;
	char *pbegin = nullptr, *pend = nullptr, *ptemp = nullptr;
	char *contentLen;
	FastDFS fastDFS;
	FastCGI fastCGI;
	MySql mysql;
	Logger mylog;
	
public:
	File() {}
	~File() {}
};
