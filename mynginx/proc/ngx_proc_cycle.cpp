#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>


#include "func.h"
#include "conf_get.h"


extern void __argv_free();
extern void free_environ();


void ngx_worker_proc_cycle();
void create_worker_process(int worker_proc_num);
void ngx_worker_proc();
int ngx_demon();

int work_num_real;

int m_sigset[] = {
        SIGIO,
        SIGINT,
        SIGHUP,
        SIGUSR1,
        SIGUSR2,
        SIGCHLD,
        0
};

int ngx_master_proc_cycle(const vector<config> &con)
{
        ngx_demon();
        work_num_real = 0;

        //设置屏蔽信号集
        sigset_t new_set, old_set;
        int *p_sig = m_sigset;
        sigemptyset(&new_set);     //清空信号集
        while(*(p_sig++) )
        {
                sigaddset(&new_set, *p_sig);
        }
        if(sigprocmask(SIG_BLOCK, &new_set, &old_set) == -1)
        {
                ngx_log_error(0, "信号屏蔽字设置失败");
        }

        //子进程创建
        int worker_proc_num(0);
        for(int it = 0; it < con.size(); ++it)
        {
                if(con.at(it).con_Name == "work_process")
                {
                        worker_proc_num = con.at(it).con_Value.at(0) - '0';
                }
        }
        create_worker_process(worker_proc_num);

        sigemptyset(&old_set);
        //主进程（master  process 循环）
        while(true)
        {
                sigsuspend(&old_set);
                create_worker_process(worker_proc_num - work_num_real);
                //ngx_log_error(0, "THIS IS PARENT PROCESS, PID IS: %d, PPID IS: %d", getpid(), getppid());
                //
                sleep(2);
                //return 0;
        }
}
void create_worker_process(int worker_proc_num)
{
        while(worker_proc_num--)
        {
                ngx_worker_proc();
        }
}

void ngx_worker_proc()
{
        pid_t w_pid = fork();
        char proc_Name[] =  "nginx: worker process";
        switch(w_pid)
        {
                case -1:
                                ngx_log_error(5, "子进程创建失败");
                                break;
                case 0:
                                __argv_free();
                                set_proctitle(_argv, proc_Name);
                                ngx_worker_proc_cycle();
                                break;
                default:
                                ++work_num_real;
                                break;
        }
}
void ngx_worker_proc_cycle()
{
        sleep(5);
        while(true)
        {
                
                //ngx_log_error(0, "THIS IS CHILD PROCESS, PID IS: %d, PPID IS: %d", getpid(), getppid());
                //exit(EXIT_SUCCESS);
        }
}

int ngx_demon()
{
        switch(fork())
        {
                case -1:
                        ngx_log_stderr(0, "守护进程创建失败 ");
                case 0:
                        break;
                default:
                        free_environ();
                        __argv_free();
                        exit(EXIT_SUCCESS);
        }

        spid = getpid();
        setsid();
        umask(0);

        int n_fd = open("/dev/null", O_RDWR);
        if(n_fd == -1)
        {
                ngx_log_error(0, "黑洞文件打开失败 ");
                return -1;
        }
        if(dup2(n_fd, STDIN_FILENO) == -1)
        {
                ngx_log_error(0, "标准输入重定向失败 ");
                return -1;
        }
        if(dup2(n_fd, STDOUT_FILENO) == -1)
        {
                ngx_log_error(0, "标准输出重定向失败 ");
                return -1;
        }
        ngx_log_error(0, "修改主进程成功 ");
        return 0;
}