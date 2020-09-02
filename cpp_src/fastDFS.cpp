#include"fastDFS.h"

using namespace std;

bool FastDFS::FdfsClientInit() {
	if (fdfs_client_init(CLIENT_CONF)) {
		LOG("fdfs_client initial failed.");
		return false;
	}

	return true;
}

bool FastDFS::TrackerGetConnection() {
	this->ptrackerServer = tracker_get_connection();
	if (this->ptrackerServer == nullptr) {
		LOG("connect tracker failed.");
		return false;
	}

	return true;
}

bool FastDFS::TrackerQueryStorageStore() {
	if (tracker_query_storage_store(this->ptrackerServer,
								&this->storageServer,
								this->groupName,
								&this->storePathIndex)) {
		LOG("query storage failed.");
		return false;
	}

	return true;
}

bool FastDFS::StorageUploadByFilename1(char* fileName, char *fileId) {
	*this->groupName = '\0';
	if (storage_upload_by_filename1(this->ptrackerServer,
								&this->storageServer,
								this->storePathIndex,
								fileName, NULL, NULL,
								0, this->groupName,
								fileId)) {
		LOG("upload file failed.");
		return false;
	}
	return true;
}

bool FastDFS::TrackerCloseConnectionEx() {
	tracker_close_connection_ex(this->ptrackerServer, true);
	return true;
}

bool FastDFS::FdfsClientDestroy() {
	fdfs_client_destroy();
	return true;
}