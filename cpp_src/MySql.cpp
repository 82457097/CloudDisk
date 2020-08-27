#include"MySql.h"

bool MySql::MysqlInit() {
	m_conn = MysqlConnect();
	if (m_conn == nullptr) {
		cout << "conn == nullptr." << endl;
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
	mylog.WriteMsgToFile(mylog.m_message);
	return true;
}

MYSQL* MySql::MysqlConnect() {
	MYSQL* conn = nullptr;
	conn = mysql_init(nullptr);
	if (conn == nullptr) {
		LOG("Sql failed to connect!");
		mylog.WriteMsgToFile(mylog.m_message);
		return conn;
	}
	
	mysql_real_connect(conn, "localhost", USER, PASSWORD, DB_NAME, 0, NULL, 0);
	LOG("get conn Successful.");
	mylog.WriteMsgToFile(mylog.m_message);
	return conn;
}

bool MySql::MysqlFindAll() {

}

bool MySql::MysqlQuery(const char *sql) {
	int retVal = mysql_query(m_conn, sql);

	if (retVal) {
		cout << "query sql failed." << endl;
		return false;
	}

	LOG("query sql Successful.");
	mylog.WriteMsgToFile(mylog.m_message);
	return true;
}

bool MySql::MysqlDisconnect() {
	mysql_close(m_conn);

	cout << "Successful to disconnect!" << endl;
	return true;
}