#ifndef _NGX_LOG_H_
#define _NGX_LOG_H_

#define NGX_LOG_STDERR            0    //控制台错误【stderr】：最高级别日志，日志的内容不再写入log参数指定的文件，而是会直接将日志输出到标准错误设备比如控制台屏幕
#define NGX_LOG_EMERG             1    //紧急 【emerg】
#define NGX_LOG_ALERT             2    //警戒 【alert】
#define NGX_LOG_CRIT              3    //严重 【crit】
#define NGX_LOG_ERR               4    //错误 【error】：属于常用级别
#define NGX_LOG_WARN              5    //警告 【warn】：属于常用级别
#define NGX_LOG_NOTICE            6    //注意 【notice】
#define NGX_LOG_INFO              7    //信息 【info】
#define NGX_LOG_DEBUG             8    //调试 【debug】：最低级别

//错误日志默认目录
#define NGX_LOG_ERROR_PATH "logs/error.log"
//缓存NUFF最大场长度
#define NGX_BUFF_MAXSIZE 2048

#define NGX_CHAR_Y 1
#define NGX_CHAR_N 0

void ngx_log_error(int err, const char *fmt, ...);
void ngx_log_stderr(int err, const char *fmt, ...);

#endif