#ifndef __UPLOAD_H__
#define __UPLOAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "logger.h"
#include "log.h"

#define UP_DEBUG(fmt, x...) DEBUG("fdfs", "upload", fmt, ##x)
#define UP_INFO(fmt, x...) INFO("fdfs", "upload", fmt, ##x)
#define UP_ERROR(fmt, x...) ERROR("fdfs", "upload", fmt, ##x)
int Upload(const char* client_conf_file, const char* upload_file, char* file_id);

#endif