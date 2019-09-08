#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "func.h"

using namespace std;


#define NEED_FREE_ENVIRON 1     //申请了环境变量新内存
#define DONT_NEED_FREE_ENVIRON 0        //未申请环境变量新内存


extern u_int ngx_time_to_buf(char *buf, const char *fmt, ...);

int len_environ = 0;    //环境内存的大小
char *en_temp = nullptr;
char *free_temp;        //释放环境变量的内存指针
bool YN_FREE;           //释放标志

void move_environ()
{
       for(int i = 0; environ[i]; i++)
        {
                len_environ += strlen(environ[i]) + 1;
        }
        en_temp = new char[len_environ];
        free_temp = en_temp;
        if(en_temp == nullptr)
        {
                cout << "内存不足 ..." << endl;
                exit(EXIT_FAILURE);
        }
        else
        {
                memset(en_temp, 0 , len_environ); 
        }
        
        for(int i = 0; environ[i]; ++i)
        {
                strcpy(en_temp, environ[i]);
                environ[i] = en_temp;
                en_temp +=  strlen(environ[i]) + 1;
        }
}

void set_proctitle(char **temp_argv, char *proc_Name)
{
        char *temp = temp_argv[0];
        size_t len_argv = 0;
        for(int i = 0; temp_argv[i]; ++i)
        {
                len_argv += strlen(temp_argv[i]) + 1;
        }
        if(len_argv < strlen(proc_Name) + 1)
        {
                move_environ();
                YN_FREE = NEED_FREE_ENVIRON;
                memset(temp, 0, len_argv + len_environ);
                if(len_argv > strlen(proc_Name) + 1 + len_environ)
                {
                        cout << "进程名过长 ..." << endl;
                        exit(EXIT_FAILURE);
                }
        }
        else
        {
                memset(temp, 0, len_argv);
                YN_FREE = DONT_NEED_FREE_ENVIRON;
        }
        
        for(int i = 0; temp_argv[i]; ++i)
        {
                char *temp1 = temp_argv[i];
                memset(temp1, 0 , strlen(temp_argv[i]) + 1);
        }
        if(spid != getpid())
        {
                //工作进程设置进程名
                strcpy(temp, proc_Name);
        }
        else
        {
                //主进程设置进程名
                u_int add = ngx_time_to_buf((char *)temp, "%s", proc_Name);
                char **__argv_temp = __argv;
                for(int i = 0; i < _argc; ++i)
                {
                        add += ngx_time_to_buf((char *)(temp + add), " %s", __argv_temp[i]);
                }
                temp[add] = '\0';
        }
        
        temp_argv[1] = NULL;
}

void free_environ()
{
        if(YN_FREE == NEED_FREE_ENVIRON)
        {
                delete []free_temp;
        }
        YN_FREE = DONT_NEED_FREE_ENVIRON;
}