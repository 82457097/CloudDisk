#include"MySql.h"

bool MySql::MysqlInit() {
	m_conn = MysqlConnect();
	if (m_conn == nullptr) {
		return false;
	}
	
/*
	char sql[SQL_LEN] = { '\0' };
	snprintf(sql, SQL_LEN, "create database %s charset=utf8", DB_NAME);

	if (!MysqlQuery(sql)) {
		cout << "query failed." << endl;
		//MysqlDisconnect();
		return false;
	}
*/
	LOG("initial Successful.");
	return true;
}

MYSQL* MySql::MysqlConnect() {
	MYSQL* conn = nullptr;
	conn = mysql_init(nullptr);
	if (conn == nullptr) {
		LOG("Sql failed to connect!");
		return conn;
	}
	
	mysql_real_connect(conn, "localhost", USER, PASSWORD, DB_NAME, 0, NULL, 0);
	LOG("get conn Successful.");
	return conn;
}

bool MySql::MysqlFindAll() {

}

bool MySql::MysqlQuery(const char *sql) {
	int retVal = mysql_query(m_conn, sql);

	if (retVal) {
		LOG("query sql failed.");
		return false;
	}

	LOG("query sql Successful.");
	return true;
}

bool MySql::MysqlDisconnect() {
	mysql_close(m_conn);

	LOG("Successful to disconnect!");
	return true;
}