#pragma once
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

#define LOG_FILE_NAME "log.txt"

#define LOG(fmt, args...)	do{\
			char _buf[1024] = {0};\
			snprintf(_buf, sizeof(_buf), "[%s:%s:%d][LOG_NORMAL]" fmt "\n",__FILE__,__FUNCTION__,__LINE__, ##args);\
			Logger::Log(string(_buf));\
		}while(false)


class Logger {
public:
	Logger();
	//Logger(const std::string& plogfile);
	static void Log(const std::string& plogMsg);
	void WriteMsgToFile(string& pMsg) {
		m_logfile << pMsg << endl;
		m_message = "";
	}
	~Logger();
	
public:
	std::fstream m_logfile;
	static string m_message;
};

static std::string TimeStamp() {
	char str[9];
	time_t a = time(0);
	struct tm* b = localtime(&a);
	strftime(str, 9, "%H:%M:%S", b);

	return str;
}

static std::string DateStamp() {
	char str[11];
	time_t a = time(0);
	struct tm* b = localtime(&a);
	strftime(str, 11, "%Y.%m.%d", b);

	return str;
}

