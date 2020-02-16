#ifndef __DELETE_H__
#define __DELETE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include "fdfs_client.h"
#include "logger.h"
#include "log.h"

#define DEL_DEBUG(fmt, x...) DEBUG("fdfs", "upload", fmt, ##x)
#define DEL_INFO(fmt, x...) INFO("fdfs", "upload", fmt, ##x)
#define DEL_ERROR(fmt, x...) ERROR("fdfs", "upload", fmt, ##x)
int DeleteFile(const char* client_conf_file, char* file_id);
#endif