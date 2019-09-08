#include <string.h>
#include <time.h>
#include <string>
#include <stdarg.h>
#include <unistd.h>
#include <ostream>


#include "conf_get.h"
#include "ngx_log.h"


extern u_int ngx_time_to_buf(char *buf, const char *fmt, ...);
extern u_int ngx_printf_to_buf(char *buf, const char *fmt, va_list v_argv);
extern u_int ngx_int_to_buf(char *buf, 
                                                int var, int b_x, 
                                                int s_x, 
                                                u_int add_num, 
                                                u_int i_width, 
                                                char c_temp);

typedef struct{
        ofstream log_file_ostream;
        int log_level;
}ngx_log_t;

static char level[][20] = {
        "[stderr]",    //0：控制台错误
        "[emerg]",     //1：紧急
        "[alert]",     //2：警戒
        "[crit]",      //3：严重
        "[error]",     //4：错误
        "[warn]",      //5：警告
        "[notice]",    //6：注意
        "[info]",      //7：信息
        "[debug]"      //8：调试
};

ngx_log_t log_t;

void ngx_log_stderr(int err, const char *fmt, ...)
{
        va_list v_argv;
        char err_buf[NGX_BUFF_MAXSIZE];

        //初始化BUF开头：   nginx: 
        memset(err_buf, 0, NGX_BUFF_MAXSIZE);
        memcpy(err_buf, "nginx: ", 7);
        err_buf[NGX_BUFF_MAXSIZE - 1] = '\0';
        //绑定可变参到v-srgv
        va_start(v_argv, fmt);
        //将剩余的字符追加到BUF,返回追加个数
        u_int add_num = ngx_printf_to_buf(err_buf + 7, fmt, v_argv);
        //释放可变参列表v_argv
        va_end(v_argv);

        if(add_num + 7 >= NGX_BUFF_MAXSIZE)
        {
                err_buf[NGX_BUFF_MAXSIZE - 1] = '\0';
        }
        else
        {
                err_buf[add_num + 7 ] = '\0'; 
        }
        write(STDERR_FILENO, err_buf, 7 + add_num);
        write(STDERR_FILENO, "\n", 1);
}

void ngx_log_error(int err, const char *fmt, ...)
{
        va_list v_argv;
        char err_buf[NGX_BUFF_MAXSIZE];
        
        //初始化BUF开头：   当前系统时间
        time_t local_t;
        struct  tm *m_time;
        char m_stime;
        time(&local_t);
        m_time = localtime(&local_t);
        memset(err_buf, 0, NGX_BUFF_MAXSIZE);
        err_buf[NGX_BUFF_MAXSIZE - 1] = '\0';
        
        u_int add_num = ngx_time_to_buf(err_buf, "%04d年%02d月%02d日-%02d:%02d:%02d", 
                                                                                                m_time->tm_year + 1900,
                                                                                                m_time->tm_mon + 1,
                                                                                                m_time->tm_mday,
                                                                                                m_time->tm_hour,
                                                                                                m_time->tm_min,
                                                                                                m_time->tm_sec);

        //追加错误等级以及进程号
        add_num += ngx_time_to_buf(err_buf + add_num, " %s %d %d: ", level[err], getpid(), getppid());

        //绑定可变参到v-srgv
        va_start(v_argv, fmt);
        //将信息追加到BUF,返回追加个数
        add_num += ngx_printf_to_buf(err_buf + add_num, fmt, v_argv);
        //释放可变参列表v_argv
        va_end(v_argv);

        if(add_num  >= NGX_BUFF_MAXSIZE)
        {
                err_buf[NGX_BUFF_MAXSIZE - 1] = '\0';
        }
        else
        {
                err_buf[add_num ] = '\0'; 
        }
        log_t.log_file_ostream << err_buf << endl;
}

void ngx_log_init(const vector<config> &con_List)
{
        string log_file = NGX_LOG_ERROR_PATH;
        for(int i = 0; i < con_List.size(); ++i)
        {
                if(con_List.at(i).group == "log" && con_List.at(i).con_Name == "log_file")
                {
                        log_file = con_List.at(i).con_Value;
                }
                else if(con_List.at(i).group == "log" && con_List.at(i).con_Name == "max_err_level")
                {
                        log_t.log_level = (int)con_List.at(i).con_Value.at(0) - '0';
                }
        }
        log_t.log_file_ostream.open(log_file, ios::app);
        if(log_t.log_file_ostream.fail())
        {
                ngx_log_stderr(0, "日志文件载入失败");
        }
        //log_t.log_file_ostream.close();
}
void free_ngx_log()
{
        log_t.log_file_ostream.close();
}