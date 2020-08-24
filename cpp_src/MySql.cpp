#include"MySql.h"

bool MySql::MysqlInit() {
	conn = MysqlConnect();
	if (conn == nullptr) {
		cout << "conn == nullptr." << endl;
		return false;
	}
	char sql[SQL_LEN] = { "\0" };
	snprintf(sql, SQL_LEN, "create database %s charset=utf8", DB_NAME);
	//cout << sql << endl;
	if (!MysqlQuery(sql)) {
		cout << "query failed." << endl;
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
	int retVal = mysql_query(conn, sql);
	cout << retVal << endl;
	if (retVal) {
		return false;
	}

	return true;
}

bool MySql::MysqlDisconnect() {
	mysql_close(conn);

	cout << "Succesed to disconnect!" << endl;
	return true;
}