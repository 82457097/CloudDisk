#pragma once
#include"fdfs_client.h"
#include<iostream>
#include<cstdlib>
#include"logger.h"
#define CLIENT_CONF "/etc/fdfs/client.conf"

class FastDFS {
private:
	int storePathIndex;
	char groupName[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo *ptrackerServer;
	ConnectionInfo storageServer;

public:
	FastDFS() {}
	~FastDFS() {}
	bool FdfsClientInit();

	bool TrackerGetConnection();

	bool TrackerQueryStorageStore();

	bool StorageUploadByFilename1(char* fileName, char *fileId);

	bool TrackerCloseConnectionEx();

	bool FdfsClientDestroy();
};
