#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <pthread.h>

#include "log.h"

// 创建日志文件路径
static int create_log_path(char *path, char *module_name, char *proc_name)
{
    char top_dir[1024] = {"."};
    char second_dir[1024] = {"./logs"};
    char third_dir[1024] = {0};
    char y_dir[1024] = {0};
    char m_d_dir[1024] = {0};
    // 当前时间
    time_t t;
    time(&t);
    struct tm* now = localtime(&t);
    // 完整路径，eg: ./logs/module_name/2019/0414/proc_name.log
    snprintf(path, 1024, "./logs/%s/%04d/%02d%02d/%s.log",
                          module_name, 
                          now->tm_year + 1900, 
                          now->tm_mon + 1,
                          now->tm_mday,proc_name);
    // 分级目录
    sprintf(third_dir, "%s/%s", second_dir, module_name);
    sprintf(y_dir, "%s/%04d/", third_dir, now->tm_year+1900);
    sprintf(m_d_dir, "%s/%02d%02d/", y_dir, now->tm_mon+1, now->tm_mday);
    // 创建目录
    if (access(top_dir, F_OK) == -1) {
        if (mkdir(top_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "create %s failed!\n", top_dir);    
        } else if (mkdir(second_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "%s OK, but create %s failed!\n", top_dir, second_dir);
        } else if (mkdir(third_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "%s OK, but create %s failed!\n", top_dir, third_dir);
        } else if (mkdir(y_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "%s OK, but create %s failed!\n", top_dir, y_dir);                                                     
        } else if (mkdir(m_d_dir, ORDINARY_PERMISSION) == -1) {                                                             
                fprintf(stderr, "%s OK, but create %s failed!\n", top_dir, m_d_dir);                                                     
        }
    } else if (access(second_dir, F_OK) == -1) {
        if (mkdir(second_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "create %s failed!\n", second_dir);
        } else if (mkdir(third_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "%s OK, but create %s failed!\n", second_dir, third_dir);
        } else if (mkdir(y_dir, ORDINARY_PERMISSION) == -1) {
                fprintf(stderr, "%s OK, but create %s failed!\n", second_dir, y_dir);
        } else if (mkdir(m_d_dir, ORDINARY_PERMISSION) == -1) {
                fprintf(stderr, "%s OK, but create %s failed!\n", second_dir, m_d_dir);
        }
    } else if (access(third_dir, F_OK) == -1) {
        if (mkdir(third_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "create %s failed!\n", third_dir);
        } else if (mkdir(y_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "%s OK, but create %s failed!\n", third_dir, y_dir);
        } else if (mkdir(m_d_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "%s OK, but create %s failed!\n", third_dir, m_d_dir);
        } 
    } else if (access(y_dir, F_OK) == -1) {
        if (mkdir(y_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "create %s failed!\n", y_dir);
        } else if (mkdir(m_d_dir, ORDINARY_PERMISSION) == -1) {
            fprintf(stderr, "%s:create %s failed!\n", y_dir, m_d_dir);
        }
    } else if (access(m_d_dir, F_OK) == -1) {
        if(mkdir(m_d_dir, ORDINARY_PERMISSION)) {
            fprintf(stderr, "create %s failed!\n", m_d_dir);
        } 
    }

    return 0;
}

// 写日志
static int write_log(char* path, char* buf)
{
    int fd = open(path, O_RDWR|O_CREAT|O_APPEND, ORDINARY_PERMISSION);
    // 失败
    if (write(fd, buf, strlen(buf)) != (int)strlen(buf)) {
        fprintf(stderr, "write log error!\n");
        close(fd);
        return -1;
    }
    // 成功
    close(fd);
    return 0;
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// 对外封装使用逻辑
int usage(char* kind, char* module_name, char* proc_name, const char* filename,
          int line, const char* funcname, char *fmt, ...)
{
    char msg[4096] = {0};
    char buf[4096] = {0};
    char filepath[1024] = {0};

    time_t t = 0;
    time(&t);
    struct tm* now = localtime(&t);

    va_list valist;                                                                                 
    va_start(valist, fmt);
    vsprintf(msg, fmt, valist);
    va_end(valist);
    // 日志格式：[info] 2019/04/14 22:37:57, main[7]: test a log
    snprintf(buf, 4096, "[%s] %04d/%02d/%02d %02d:%02d:%02d %s-%s[%d]: %s\n",
                         kind,
                         now->tm_year + 1900, 
                         now->tm_mon + 1,
                         now->tm_mday, now->tm_hour, 
                         now->tm_min, now->tm_sec,
                         filename, funcname, line, msg);
    create_log_path(filepath, module_name, proc_name);
    
    pthread_mutex_lock(&lock);
    write_log(filepath, buf);
    pthread_mutex_unlock(&lock);

    return 0;
}

void not_do_anything() {}