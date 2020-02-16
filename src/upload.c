#include "upload.h"


int Upload(const char* client_conf_file, const char* upload_file, char* file_id) {
    int result;
    int store_path_index;
    char group_name[FDFS_GROUP_NAME_MAX_LEN+1];
    ConnectionInfo *p_tracker_server = NULL;
    ConnectionInfo storage_server;
    
    UP_DEBUG("%s", "初始化配置");
    if ((result = fdfs_client_init(client_conf_file)) != 0) {
        UP_ERROR("%s", "fdfs_client 初始化失败");
        return result;
    }

    UP_DEBUG("%s", "连接 tracker");
    p_tracker_server = tracker_get_connection();
    if (p_tracker_server == NULL) {
        UP_ERROR("%s", "tracker 连接失败");
        fdfs_client_destroy();
        return errno != 0 ? errno: ECONNABORTED;
    }

    UP_DEBUG("%s", "查询存储信息");
    if ((result = tracker_query_storage_store(p_tracker_server,
                                              &storage_server,
                                              group_name,
                                              &store_path_index)) !=0 )
    {
        UP_DEBUG("%s", "进入查询失败");
        fdfs_client_destroy();
        UP_ERROR("%s %s", "tracker_query_storage_store 失败", STRERROR(result));
        return result;

    }

    UP_DEBUG("%s", "查询结束");
    *group_name = '\0';
    UP_DEBUG("%s", "上传文件");
    result = storage_upload_by_filename1(p_tracker_server, 
                                         &storage_server, 
                                         store_path_index, 
                                         upload_file, NULL, 
                                         NULL, 0, group_name, 
                                         file_id);
    if (result == 0) {
        UP_INFO("文件上传成功，file id 为：%s", file_id);
    } else {
        UP_ERROR("文件上传失败，错误号：%d 错误信息为:%s", result, STRERROR(result));
    }

    UP_DEBUG("%s", "完成上传");
    tracker_close_connection_ex(p_tracker_server, true);
    fdfs_client_destroy();
    UP_DEBUG("%s", "完成资源释放");

    return result;
}