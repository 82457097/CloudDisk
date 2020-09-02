#pragma once
#include<mysql/mysql.h>
#include<iostream>
#include"logger.h"

using namespace std;

#define	USER			"root"
#define	PASSWORD		"qwj19961202"
#define	DB_NAME			"filedata"
#define	TABLE_NAME		"data"
#define	SQL_LEN			2048

class MySql {
public:
	MySql() {}
	~MySql() {}
	bool MysqlInit();
	MYSQL* MysqlConnect();
	bool MysqlFindAll();
	bool MysqlQuery(const char *sql);
	bool MysqlDisconnect();

private:
	MYSQL* m_conn;
};
