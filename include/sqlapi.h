#ifndef __SQLAPI_H__
#define __SQLAPI_H__

#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include "log.h"


#define SQL_DEBUG(fmt, x...) DEBUG("fdfs", "sqlapi", fmt, ##x)
#define SQL_INFO(fmt, x...) INFO("fdfs", "sqlapi", fmt, ##x)
#define SQL_ERROR(fmt, x...) ERROR("fdfs", "sqlapi", fmt, ##x)

typedef struct {
    int id;
    char filename[1024];
    char  fileid[1024];
} YOU_LINE_DATA;

void MysqlInit(char* dbname, char* tablename);
MYSQL* MysqlConnect(char* usernmae, char* password, char*dbname);
int MysqlFindall(MYSQL* conn, const char* sql, YOU_LINE_DATA* data);
int MysqlExecute(MYSQL* conn, const char* sql);
void MysqlDisconnect(MYSQL* conn);

#endif