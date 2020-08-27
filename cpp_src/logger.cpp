#include"logger.h"
#include<iostream>
#include<string>

using namespace std;

string Logger::m_message = "";

Logger::Logger() {
	string plogfile = (LOG_FILE_NAME);
	std::fstream filetester(plogfile.c_str(), std::ios::in);
	if (filetester.is_open()) {
		filetester.close();
		m_logfile.open(plogfile.c_str(), std::ios::out | std::ios::app);
	} else {
		m_logfile.open(plogfile.c_str(), std::ios::out);
	}

	Log("Session opened.\n");
	this->WriteMsgToFile(m_message);
}

Logger::~Logger() {
	Log("Session closed.\n");
}

void Logger::Log(const std::string& plogMsg) {

	m_message += "[" + DateStamp() + "] ";
	m_message += "[" + TimeStamp() + "] ";

	m_message += plogMsg;
	cout << m_message << endl;
}


