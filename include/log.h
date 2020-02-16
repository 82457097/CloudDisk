#ifndef __LOG_H__
#define __LOG_H__

#include <pthread.h>

#define ORDINARY_PERMISSION 0777

extern pthread_mutex_t lock;

static int create_log_path(char *path, char *module_name, char *proc_name);
static int write_log(char* path, char* buf);
void not_do_anything();
int usage(char* kind, char* module_name, char* proc_name, const char* filename,
          int line, const char* funcname, char *fmt, ...);

#define NEED_DEBUG 1
#define NEED_INFO 1
#define NEED_ERROR 1
#define FILE_LOG  // 日志输出 或是 标准输出 开关

#ifdef FILE_LOG
#define INFO(module_name, proc_name, x...) \
            usage("info", module_name, proc_name, \
                  __FILE__, __LINE__, __FUNCTION__, ##x)
/***************************************************************/
#if NEED_DEBUG
#define DEBUG(module_name, proc_name, x...) \
            usage("debug", module_name, proc_name, \
                  __FILE__, __LINE__, __FUNCTION__, ##x)
#else
#define DEBUG(module_name, proc_name, x...) not_do_anything()
#endif
/***************************************************************/
#define ERROR(module_name, proc_name, x...) \
            usage("error", module_name, proc_name, \
                  __FILE__, __LINE__, __FUNCTION__, ##x)
#else

#pragma message "You don't enable file log."
#include <stdarg.h>

/***************************************************************/
#if NEED_INFO
#define INFO(module_name, proc_name, fmt, ...) \
            printf("[info] %s %s-%s[%d]: " fmt "\n", \
                   __TIME__, __FILE__, __FUNCTION__, \
                   __LINE__, __VA_ARGS__)
#else
#define INFO(module_name, proc_name, fmt, ...) not_do_anything()
#endif
/***************************************************************/
#if NEED_DEBUG
#define DEBUG(module_name, proc_name, fmt, ...) \
            printf("[debug] %s %s-%s[%d]: " fmt "\n", \
                   __TIME__, __FILE__, __FUNCTION__, \
                   __LINE__, __VA_ARGS__)
#else
#define DEBUG(module_name, proc_name, fmt, ...) not_do_anything()
#endif
/***************************************************************/
#if NEED_ERROR
#define ERROR(module_name, proc_name, fmt, ...) \
            printf("[error] %s %s-%s[%d]: " fmt "\n", \
                   __TIME__, __FILE__, __FUNCTION__, \
                   __LINE__, __VA_ARGS__)
#else
#define ERROR(module_name, proc_name, fmt, ...) not_do_anything()
#endif
/***************************************************************/
#endif
#endif