#include"logger.h"
#include<iostream>
#include<string>

using namespace std;

fstream Logger::m_logfile(LOG_FILE_NAME, std::ios::out | std::ios::app);

Logger::Logger() {
	Log("Session opened.\n");
}

Logger::~Logger() {
	Log("Session closed.\n");
}

void Logger::Log(const string& logMsg) {
	string time;
	time += "[" + DateStamp() + "] ";
	time += "[" + TimeStamp() + "] ";

	time += logMsg;
	cout << time << endl;
	m_logfile << time << endl;
}

string Logger::TimeStamp() {
	char str[9];
	time_t a = time(0);
	struct tm* b = localtime(&a);
	strftime(str, 9, "%H:%M:%S", b);

	return str;
}

string Logger::DateStamp() {
	char str[11];
	time_t a = time(0);
	struct tm* b = localtime(&a);
	strftime(str, 11, "%Y.%m.%d", b);

	return str;
}


