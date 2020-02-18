#include "download.h"


int Download(const char* client_conf_file, const char* file_id, char* filename) {
    int result;
	ConnectionInfo *p_tracker_server = NULL;
	int64_t file_size;
	int64_t file_offset = 0;
	int64_t download_bytes = 0;

    DOWN_DEBUG("%s", "初始化配置文件");
	if ((result = fdfs_client_init(client_conf_file)) != 0) {
		return result;
	}
    
    DOWN_DEBUG("%s", "连接 tracker");
	p_tracker_server = tracker_get_connection();
	if (p_tracker_server == NULL) {
        DOWN_DEBUG("%s", "tracker 失败");
		fdfs_client_destroy();
		return errno != 0 ? errno : ECONNREFUSED;
	}

    DOWN_DEBUG("%s", "开始下载文件");
	result = storage_do_download_file1_ex(p_tracker_server, NULL, 
                                          FDFS_DOWNLOAD_TO_FILE, file_id, 
                                          file_offset, download_bytes, 
                                          &filename, NULL, &file_size);
	if (result == 0) {
        DOWN_INFO("文件下载成功，file id 为：%s", file_id);
	} else {
        DOWN_ERROR("文件下载失败，错误号：%d 错误信息为:%s", result, STRERROR(result));
    }

    DOWN_DEBUG("%s", "断开 tracker 连接");
	tracker_close_connection_ex(p_tracker_server, true);
	fdfs_client_destroy();
	return 0;
}