
#include "func.h"
#include "ngx_socket.h"

mSocket::mSocket()
{
        //构造函数
        key = "Net";
        listen_port_num = 0;
}

//析构函数
mSocket::~mSocket()
{
        
}

//开始监听端口
void mSocket::ngx_listen_init( vector<config> &vec)
{
        int listen_port = 0;            //临时存放端口
        socket_fd_ptr p;        //指向new的 socket_fd_t

        for(vector<config>::iterator it = vec.begin(); it != vec.end(); it++)
        {
                if(it->group == key && it->con_Name == "ListenPNumber" ) 
                {
                        for(int i = 0; i < it->con_Value.length(); ++i)
                        {
                                listen_port_num *= 10;
                                listen_port_num += it->con_Value.at(i) - '0';
                        }
                }
                if(it->group == key && it->con_Name == "worker_connections" ) 
                {
                        for(int i = 0; i < it->con_Value.length(); ++i)
                        {
                                mConnection_number *= 10;
                                mConnection_number += it->con_Value.at(i) - '0';
                        }
                }
                if(it->group == key && it->con_Name.substr(0,10) == "ListenPort"  && listen_port_num--) 
                {
                        listen_port = 0;
                        p = new socket_fd_t;
                        for(int i = 0; i < it->con_Value.length(); ++i)
                        {
                                listen_port *= 10;
                                listen_port += it->con_Value.at(i) - '0';
                        }
                        p->listen_port = listen_port;
                        p->socketfd = ngx_listen(listen_port);
                        listenPort_vec.push_back(p);
                }
        }
}

//建立并返回相应port的socketfd
int mSocket::ngx_listen(int listern_port)
{
        sockaddr_in addr;
        /*建立socket所需结构体，给bind()使用时，必须强制转换为sockaddr类型
                struct sockaddr_in
                {
                __SOCKADDR_COMMON (sin_);
                in_port_t sin_port;			// Port number. 
                struct in_addr sin_addr;		// Internet address. 

                // Pad to size of `struct sockaddr'.  
                unsigned char sin_zero[sizeof (struct sockaddr) -
			   __SOCKADDR_COMMON_SIZE -
			   sizeof (in_port_t) -
			   sizeof (struct in_addr)];
                };
        */
       //给addr赋值，建立socket的相关参数（端口，IP，协议等）    
       // 0.0.0.0 = INADDR_ANY
        //192.168.1.146 = 0xc0a80192
        memset(&addr, 0, sizeof(sockaddr_in));          //初始化内存，养成好习惯
        addr.sin_addr.s_addr = htonl(0xc0a80192);
        addr.sin_port = htons(listern_port);
        addr.sin_family = AF_INET;

        int sockfd = socket(AF_INET, SOCK_STREAM , 0);          //建立socket，失败则打印日志直接退出，内存由操作系统回收
        if(sockfd == -1)
        {
                ngx_log_error(4, "socket建立失败，检查内存大小");
                exit(EXIT_FAILURE);
        }
         int reuseaddr = 1;
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const void *) &reuseaddr, sizeof(reuseaddr)) == -1)
        {
                ngx_log_error(5, "setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const void *) &reuseaddr, sizeof(reuseaddr)失败");
                exit(EXIT_FAILURE);
        }
        if(bind(sockfd, (sockaddr *)&addr, sizeof(sockaddr_in)) == -1)                  //socket与端口绑定，失败则打印日志直接退出，内存由操作系统回收
        {
                ngx_log_stderr(4, "bind失败，socket：%d，port：%d，端口已被使用", sockfd, listern_port);
                ngx_log_error(4, "bind失败，socket：%d，port：%d，端口已被使用", sockfd, listern_port);
                exit(EXIT_FAILURE);
        }
        else
        {
                ngx_log_error(8, "bind成功，socket：%d，port：%d", sockfd, listern_port);
        }
        if(listen(sockfd, 500) == -1)
        {
                ngx_log_error(4, "端口%d监听失败", listern_port);
                exit(EXIT_FAILURE);
        }
        else
        {
                ngx_log_error(8, "端口%d监听成功", listern_port);
        }
        return sockfd;
}

//释放存放socket的vector和对应的socket
int mSocket::close_listen_fd()
{
        for(int i = 0; i < listenPort_vec.size(); ++i)
        {
                close(listenPort_vec.at(i)->socketfd);          //关闭socket的fd
                delete listenPort_vec.at(i);            //
        }
}