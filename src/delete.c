#include "delete.h"


int DeleteFile(const char* client_conf_file,
                char* file_id)
{
    int result;
	ConnectionInfo *p_tracker_server = NULL;
	
    DEL_DEBUG("%s", "初始化配置");
	if ((result=fdfs_client_init(client_conf_file)) != 0) {
		return result;
	}

    DEL_DEBUG("%s", "连接 tracker");
	p_tracker_server = tracker_get_connection();
	if (p_tracker_server == NULL) {
        DEL_INFO("%s", "tracker 连接失败");
		fdfs_client_destroy();
		return errno != 0 ? errno : ECONNREFUSED;
	}

    DEL_DEBUG("%s", "删除文件");
    result = storage_delete_file1(p_tracker_server, NULL, file_id);
	if (result == 0) {
        DEL_INFO("文件删除成功，file id 为：%s", file_id);
	} else {
        DEL_ERROR("文件删除失败，错误号：%d 错误信息为:%s", result, STRERROR(result));
    }

    DEL_DEBUG("%s", "释放连接");
	tracker_close_connection_ex(p_tracker_server, true);
	fdfs_client_destroy();

	return result;
}