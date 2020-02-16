#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include "upload.h"
#include "log.h"
#include "sqlapi.h"

#define CLIENT_CONF "/etc/fdfs/client.conf"
#define    USERNAME "root"
#define    PASSWORD "123456"
#define          DB "youguan"
#define  TABLE_DATA "data"

#define _upload(filepath, fileid) Upload(CLIENT_CONF, filepath, fileid)

#define MA_DEBUG(fmt, x...) DEBUG("fdfs", "main", fmt, ##x)
#define MA_INFO(fmt, x...) INFO("fdfs", "main", fmt, ##x)
#define MA_ERROR(fmt, x...) ERROR("fdfs", "main", fmt, ##x)

// static void my_usage();

static int ExtractDataAndSave(char* begin, char* end, int len, char* filename);

static char* GetPointerOfBorder(char* content, int len, char* boundary);

static void SaveToMysql(char* filename, char* fileid);

#endif