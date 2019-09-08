#ifndef _FUNC_H_
#define _FUNC_H_

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "conf_get.h"   //配置环境获取类

using namespace std;



extern void move_environ();     //缓存环境变量
extern void set_proctitle(char **, char *);     //修改进程标题
extern void free_environ();     //释放暂存的环境变量
extern void free_ngx_log();     //是放日志文件FD
extern void ngx_log_stderr(int err, const char *fmt, ...);      //标准错误输出函数，即输出到屏幕
extern void ngx_log_error(int err, const char *fmt, ...);       //日志文件函数
extern void ngx_log_init(const vector<config> &);       //日志初始化函数
extern int ngx_signal_init();   //注册信号函数
extern int ngx_master_proc_cycle(const vector<config> &con);    //主进程及工作进程循环函数
extern int sim_server();        //server测试函数


extern char **_argv;    //主函数启动参数列表
extern char **__argv;   //暂存的主函数启动参数列表
extern int _argc;       //主函数启动参数个数
extern pid_t spid;      //主进程的组PID




#endif