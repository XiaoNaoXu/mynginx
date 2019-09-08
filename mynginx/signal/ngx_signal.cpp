#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#include "ngx_log.h"

extern int work_num_real;
extern void free();

static void test(int signo, siginfo_t *siginfo, void *ucontext);
static void __sigchld(int signo, siginfo_t *siginfo, void *ucontext);
static void __sighup(int signo, siginfo_t *siginfo, void *ucontext);
static void __siguser1(int signo, siginfo_t *siginfo, void *ucontext);
static void __siguser2(int signo, siginfo_t *siginfo, void *ucontext);
static void __sigint(int signo, siginfo_t *siginfo, void *ucontext);


typedef struct{
        u_int sigNo;
        const char *sigName;
        void (*sig_handler)(int signo, siginfo_t *siginfo, void *ucontext);
}ngx_signal_t;


ngx_signal_t sig_set[] = {
        {SIGHUP, "SIGHUP", __sighup},
        
        {SIGINT, "SIGINT", __sigint},
        {SIGQUIT, "SIGQUIT", __sigint},
        {SIGILL, "SIGILL", __sigint},
        {SIGUSR1, "SIGUSR1", __siguser1},
        {SIGUSR2, "SIGUSR2",__siguser2},
        {SIGCHLD, "SIGCHLD", __sigchld},
        {0, nullptr, nullptr}
};


int ngx_signal_init()
{
        ngx_signal_t *sig_temp = sig_set;
        struct sigaction sa;
        while(sig_temp->sigNo)
        {                
                memset(&sa, 0, sizeof(struct sigaction));
                if(sig_temp->sig_handler != nullptr)
                {
                        sa.sa_sigaction = sig_temp->sig_handler;
                        sa.sa_flags = SA_SIGINFO;
                }
                else
                {
                        sa.sa_handler = SIG_IGN;
                }
                if(sigaction(sig_temp->sigNo, &sa, NULL) == -1)
                {
                        ngx_log_stderr(2, "%s 设置失败 ", sig_temp->sigName);
                        ngx_log_error(2, "%s 设置失败 ", sig_temp->sigName);
                }
                else
                {
                        //ngx_log_stderr(0, "%s 设置成功 ", sig_temp->sigName);
                }
                sig_temp ++;
        }
        return 0;
}

static void __sigchld(int signo, siginfo_t *siginfo, void *ucontext)
{
        int status;
        --work_num_real;
        ngx_log_error(7, "收到SIGHUP信号，__sigchld()启动");
        wait(&status);
}


static void __sighup(int signo, siginfo_t *siginfo, void *ucontext)
{
        int status;
        --work_num_real;
        ngx_log_error(7, "收到SIGHUP信号，__sigchld()启动");
        wait(&status);
}

static void __siguser1(int signo, siginfo_t *siginfo, void *ucontext)
{
        ngx_log_error(7, "收到SIGUSR1信号，__siguser1()启动");
}

static void __siguser2(int signo, siginfo_t *siginfo, void *ucontext)
{
        int status;
        --work_num_real;
        ngx_log_error(7, "收到SIGHUP信号，__sigchld()启动");
        wait(&status);
}

static void __sigint(int signo, siginfo_t *siginfo, void *ucontext)
{
        std::cout << "已结束 "<< signo << std::endl;
        free();
        exit(EXIT_FAILURE);
}