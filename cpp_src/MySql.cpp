#include"MySql.h"

bool MySql::MysqlInit() {
	conn = MysqlConnect();
	if (conn == nullptr) {
		return false;
	}
	char sql[SQL_LEN] = { "\0" };
	snprintf(sql, SQL_LEN, "create database %s charset=utf8", DB_NAME);
	if (!MysqlQuery(sql)) {
		return false;
	}
	
	return true;
}

MYSQL* MySql::MysqlConnect() {
	conn = mysql_init(nullptr);
	if (conn == nullptr) {
		cout << "Sql failed to connect!" << endl;
		return conn;
	}

	mysql_real_connect(conn, "localhost", USER, PASSWORD, DB_NAME, 0, NULL, 0);
	return conn;
}

bool MySql::MysqlFindAll() {

}

bool MySql::MysqlQuery(const char *sql) {
	if (mysql_query(conn, sql)) {
		return false;
	}

	return true;
}

bool MySql::MysqlDisconnect() {
	if (mysql_close(conn)) {
		cout << "Failed to disconnect!" << endl;
		return false;
	}

	cout << "Succesed to disconnect!" << endl;
	return true;
}