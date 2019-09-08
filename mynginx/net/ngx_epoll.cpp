#include "ngx_socket.h"
#include "func.h"


void mSocket::ngx_epoll_init()
{
        //epoll 创建
        epoll_handler = epoll_create(m_worker_connections);
        if(epoll_handler == -1)
        {
                ngx_log_error(2, "epoll初始化时，epoll_create失败，返回-1");
                exit(EXIT_FAILURE);
        }

        //连接池创建
        mConnection_number = m_worker_connections;      //连接池长度
        connection_head = new connection_t[mConnection_number];         //连接池首地址

        //创建空闲链
        int int_temp = mConnection_number;      //临时变量      
        connection_pty conn_temp = NULL;        //临时连接池指针
        while(int_temp)
        {
                connection_head[int_temp].fd = -1;
                connection_head[int_temp].instance = 1;
                connection_head[int_temp].data = conn_temp;
                connection_head[int_temp].iCurrsequence = 0;
                conn_temp = &connection_head[int_temp];
        }

        connection_head = conn_temp;
        free_mConnection_number = mConnection_number;

        for(vector<socket_fd_ptr>::iterator it = listenPort_vec.begin(); it != listenPort_vec.end(); ++it)
        {
                (*it)->connection = get_free_connect();
        }
}