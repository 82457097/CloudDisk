#include"showFiles.h"

int main() {
	MysqlInit(DB, TABLE_DATA);
	
	while(FCGI_Accept() >= 0) {
		int bufLen = 0;
		char *contentLen = getenv("CONTENT_LENGTH");
		printf("Content-type:text/html\r\n\r\n");
		if(contentLen != NULL) {
			bufLen = strtol(contentLen, NULL, 10);
		}

		if(bufLen <= 0) {
			printf("<br>没有接收到任何数据！<br>\n");
		}
		else {
			char *data = (char*)malloc(bufLen);
			memset(data, 0, bufLen);
			int ret;
			ret = FCGI_fread(data, bufLen, 1, stdin);
			if("value1=1&value2=1&value" == *data) {
				printf("<br>OK!<br>");
				MYSQL* connect = MysqlConnect(USERNAME, PASSWORD, DB);
				FILE_DATA* fileInfo = (FILE_DATA*)malloc(sizeof(FILE_DATA)*10);
				memset(fileInfo, 0, sizeof(FILE_DATA)*10);
				char* sql = (char*)malloc(1024);
				snprintf(sql, 1024, "SELECE * FROM %s",TABLE_DATA);
				int count = MysqlFindall(connect, sql, fileInfo);
				printf("Content-type: %s\r\n\r\n", fileInfo);
				printf("<br>%s<br>", fileInfo);
				free(fileInfo);
			}
			printf("ERROR!");
			free(data);
		}
	}

	return 0;
}


