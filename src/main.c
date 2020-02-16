#include "main.h"

int main (int argc, char** argv) {
    MysqlInit(DB, TABLE_DATA);  /* 初始化数据库 */

    while (FCGI_Accept() >= 0) {
        int len = 0;
        char *content_len = getenv("CONTENT_LENGTH");
        printf("Content-type: text/html\r\n\r\n");

        if (content_len != NULL)
            len = strtol(content_len, NULL, 10);  /* str -> int */

        MA_INFO("%s: %d", "接收数据长度是", len);
        if (len <= 0) {
            printf("No data from standard input.<p>\n");
        } else {
            int ch;
            char filename[1024] = {0};
            char *file_data = (char*)malloc(len);  /* 申请缓存块 */
            char *begin = NULL, *end = NULL, *p = NULL;
            
            begin = p = file_data;  /* 指向缓存块的首地址 */
            /* 接收浏览器传来的数据 */
            MA_INFO("%s", "开始接收浏览器数据...");
            for (int i = 0; i < len; i++) {
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received "
                           "on standard input<p>\n");
                    break;
                }
                *p = ch; 
                ++p;
            }
            MA_INFO("%s", "接收完毕...");
            end = p;  /* end 指向缓存块末尾 */
            MA_INFO("%s", "解析并且保存数据");
            ExtractDataAndSave(begin, end, len, filename);  /* 解析数据 */

            /* 文件上传到fastdfs */
            MA_INFO("%s", "上传文件到 fdfs ");
            char fileid[1024] = {0};
            _upload(filename, fileid);
            MA_INFO("%s: %s", "fileid", fileid);
            printf("<br>fileid: %s\n<br>", fileid);

            // 将数据存储到数据库中
            SaveToMysql(filename, fileid);

            free(file_data);
            unlink(filename);  /* 删除文件 */
        }
    } /* while */

    return 0;
}

/*
 * multipart/form-data 表单格式如下(末尾以 '\r\n' 换行)：
------WebKitFormBoundaryrGKCBY7qhFd3TrwA\r\n
Content-Disposition: form-data; name="file"; filename="chrome.png"\r\n
Content-Type: image/png\r\n
\r\n
PNG ... content of chrome.png ...
------WebKitFormBoundaryrGKCBY7qhFd3TrwA--\r\n
 */
static int ExtractDataAndSave(char* begin, char* end, int len, char* filename) {
    char *p = NULL;
    
    /* 解析第一行 */
    char boundary[256] = {0};
    p = strstr(begin, "\r\n");
    strncpy(boundary, begin, p - begin);  /* 保存分界线至 boundary */
    MA_INFO("%s: %s", "分割线内容", boundary);
    
    /* 解析第二行  filename="chrome.png"*/
    p += 2;
    len -= (p - begin);
    MA_INFO("len: %d", len);
    begin = p;  /* begin 指向第二行 */
    char* p_last_symbol = strstr(begin, "filename=");
    p_last_symbol += strlen("filename=");  /* 指向第一个双引号 */
    char* p_next_symbol = strchr(++p_last_symbol, '"');  /* 指向第二个双引号 */
    strncpy(filename, p_last_symbol, p_next_symbol - p_last_symbol);
    MA_INFO("filename: %s", filename);
    printf("<br>filename: %s<br>", filename);

    /* 第三行、四行 */
    p = strstr(begin, "\r\n");
    p += 2;
    len -= (p - begin);  /* 剩余长度 */
    begin = p;
    p = strstr(begin, "\r\n");
    p += 4;
    len -= (p-begin);  /* 所需数据长度 */

    /* 开始正文 */ 
    begin = p;
    p = GetPointerOfBorder(begin, len, boundary);
    if (p == NULL) p = end;
    p -= 2;  /* 退两个字符：/r/n */

    /* 本地保存 */
    MA_INFO("%s", "数据开始写入本地 ...");
    int fd = open(filename, O_CREAT|O_WRONLY, 0664);
    write(fd, begin, p - begin);
    close(fd);

    return 0;
}


static char* GetPointerOfBorder(char* content, \
                                   int len, \
                                   char* boundary) {
    /* 参数校验 */
    if (content == NULL || len <= 0 || boundary == NULL)
        return NULL;
    if (*boundary == '\0') return NULL;

    int boder_len = strlen(boundary);
    char* cur = content;
    /* +1 是为了让指针走到边界线的第一个字符 */
    int full_data_len = len - boder_len + 1;
    for (int i = 0; i < full_data_len; i++) {
        if (*cur == *boundary) {
            if (memcmp(cur, boundary, boder_len) == 0)
                return cur;  /* 指针指向边界线首地址 */
        }
        ++cur;
    }
    return NULL;
}

static void SaveToMysql(char* filename, char* fileid) {
    MYSQL* conn = MysqlConnect(USERNAME, PASSWORD, DB);
    if (conn == NULL) {
        MA_ERROR("%s", "数据库连接失败, 停止保存动作");
        return;
    }

    char sql[1024*2] = {0};
    snprintf(sql, 1024*2, "insert into %s values(NULL, '%s', '%s')", TABLE_DATA, filename, fileid);
    int flag = MysqlExecute(conn, sql);
    if (flag == 0) MA_INFO("%s %s %s", "文件", filename, "元数据保存到数据库");
    else MA_INFO("%s %s %s", "文件", "filename", "保存数据库失败");
}