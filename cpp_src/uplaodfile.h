#pragma once
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<fcntl.h>
#include"file.h"

using namespace std;

class Upload : public File {
private:
	int buflen;

public:
	Upload() {}
	~Upload() {}
	bool AcceptFile();
	bool ParseDataAndSave();
	bool UploadFile(char* fileName);
	bool SaveToMysql();
};