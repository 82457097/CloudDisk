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

void Logger::Log(const std::string& plogMsg) {
	string time;
	time += "[" + DateStamp() + "] ";
	time += "[" + TimeStamp() + "] ";

	time += plogMsg;
	cout << time << endl;
	m_logfile << time << endl;
}


