#include"fastDFS.h"

using namespace std;

bool FastDFS::FdfsClientInit() {
	if (fdfs_client_init(CLIENT_CONF)) {
		return false;
	}

	return true;
}

bool FastDFS::TrackerGetConnection() {
	this->ptrackerServer = tracker_get_connection();
	if (this->ptrackerServer == nullptr) {
		return false;
	}

	return true;
}

bool FastDFS::TrackerQueryStorageStore() {
	if (tracker_query_storage_store(this->ptrackerServer,
								&this->storageServer,
								this->groupName,
								&this->storePathIndex)) {
		return false;
	}

	return true;
}

bool FastDFS::StorageUploadByFilename1(char *fileName, char *fileId) {
	*this->groupName = '\0';
	if (storage_upload_by_filename1(this->ptrackerServer,
								&this->storageServer,
								this->storePathIndex,
								fileName, NULL, NULL,
								0, this->groupName,
								fileId)) {
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