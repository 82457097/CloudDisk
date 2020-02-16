#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "logger.h"
#include "log.h"

#define DOWN_DEBUG(fmt, x...) DEBUG("fdfs", "upload", fmt, ##x)
#define DOWN_INFO(fmt, x...) INFO("fdfs", "upload", fmt, ##x)
#define DOWN_ERROR(fmt, x...) ERROR("fdfs", "upload", fmt, ##x)
int Download(const char* client_conf_file,
             const char* upload_file,
             char* file_id);

#endif