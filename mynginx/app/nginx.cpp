#include "func.h"
#include "ngx_socket.h"



void free();

char **_argv;
char **__argv;
int _argc;
pid_t spid;

void __argv_free()
{
    for(int i = 0; i < _argc; ++i)
    {
        if(__argv[i])
        {
            delete[] __argv[i];
        }
    }
    if(__argv)
    {
        delete[] __argv;
    }
}

struct te{
    int a;int d;
    
    short c;
    short c2;short c3;
    char x;
    
};

int main (int argc, char** argv)
{

    /*----------------------------------必要操作------------------------------------*/

    //配置文件读取
    conf_get *p = conf_get::getInt();
    p->Load("nginx.conf");

    //日志文件初始化
    ngx_log_init(p->con_List);
    //信号初始化
    ngx_signal_init();
    mSocket msocket;
    msocket.ngx_listen_init(p->con_List);


    //转移启动参数
    _argc = argc;
    _argv = argv;
    spid = getpid();
    __argv = NULL;
    try{
        __argv = new char*[argc + 1];
    }
    catch(const bad_alloc& e){
        ngx_log_error (0 , "转移参数[0]：申请内存失败");
    }
    memset(__argv, 0, (size_t)(argc + 1));
    for(int i = 0; i < argc; ++i)
    {
        try{
            __argv[i] = new char[strlen(_argv[i]) + 1];
        }
        catch(const bad_alloc& e){
            ngx_log_error (0 , "转移参数[1]：申请内存失败");
        }
        strcpy(__argv[i], _argv[i]);
    }

    /*
    //日志输出测试模块
    ngx_log_stderr(0, "this is a test %10s %010c", "cch", 'a');
    ngx_log_stderr(0, "this is a test %010s %xd", "cch", 60536);
    ngx_log_stderr(0, "this is a test %s %10Xd %010Xd", "cch", 1678, 1678);
    ngx_log_stderr(0, "this is a test %s %d", "cch", 123456789);
    ngx_log_stderr(0, "this is a test %s %015d", "cch", 123456789);
    ngx_log_stderr(0, "this is a test %s %15d", "cch", 123456789);
    ngx_log_stderr(0, "this is a test %s %.10f", "cch", 12345.12345);
    ngx_log_error(0, "this is a test %s %.10f", "cch", 12345.12345);
    ngx_log_error(0, "this is a test %s %.1f", "cch", 12345.12345);
    //printf("nginx: this is a test %.1f\n", 12345.12345);
    */

    //修改进程名模块
    char proc_Name[] = "nginx: master process";
    set_proctitle(_argv, proc_Name);
    //各个进程的工作
    ngx_master_proc_cycle(p->con_List);
    //sim_server();
    
    


    /*--------------------资源释放区域------------------------------------*/
    free();
    msocket.close_listen_fd();
    return 0;
}

void free()
{
    free_environ();
    free_ngx_log();
    __argv_free();
    
}