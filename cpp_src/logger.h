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
	~Logger();

	static void Log(const string& logMsg);
	static string TimeStamp();
	static string DateStamp();
	
private:
	static fstream m_logfile;
};


