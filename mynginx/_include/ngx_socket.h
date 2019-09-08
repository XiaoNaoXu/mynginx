#ifndef _NGX_SOCKET_H_
#define _NGX_SOCKET_H_

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <vector>
#include <string>

#define MAX_EVENTS 512 //最大事件总数

typedef struct socket_fd_t *socket_fd_ptr;
typedef struct connection_t *connection_pty;

/*-------------------------------------------存放socket的结构体-------------------------------------------------*/
struct socket_fd_t{

        int     socketfd;   //socket的fd

        int     listen_port;        //监听的端口

        connection_pty connection;      //socket绑定的连接池的元素
};

struct connection_t{
        int                       fd;             //套接字句柄socket
	socket_fd_ptr         listening;      //如果这个链接被分配给了一个监听套接字，那么这个里边就指向监听套接字对应的那个lpngx_listening_t的内存首地址		

	//------------------------------------	
	unsigned                  instance:1;     //【位域】失效标志位：0：有效，1：失效【这个是官方nginx提供，到底有什么用，ngx_epoll_process_events()中详解】  
	uint64_t                  iCurrsequence;  //我引入的一个序号，每次分配出去时+1，此法也有可能在一定程度上检测错包废包，具体怎么用，用到了再说
	struct sockaddr           s_sockaddr;     //保存对方地址信息用的
	//char                      addr_text[100]; //地址的文本信息，100足够，一般其实如果是ipv4地址，255.255.255.255，其实只需要20字节就够

	//和读有关的标志-----------------------
	//uint8_t                   r_ready;        //读准备好标记【暂时没闹明白官方要怎么用，所以先注释掉】
	uint8_t                   w_ready;        //写准备好标记

	//ngx_event_handler_pt      rhandler;       //读事件的相关处理方法
	//ngx_event_handler_pt      whandler;       //写事件的相关处理方法
	
	//--------------------------------------------------
	connection_pty       data;           //这是个指针【等价于传统链表里的next成员：后继指针】，指向下一个本类型对象，用于把空闲的连接池对象串起来构成一个单向链表，方便取用
};

class mSocket{
/*----------------------------------------构造函数---------------------------------------------------------------*/
        public:
                mSocket();
                ~mSocket();
        
/*----------------------------------------端口监听相关函数和结构------------------------------------------*/
        public:
                void ngx_listen_init(vector<config> &vec);     //监听端口开始
                int ngx_listen(int listern_port);       //建立并返回相应port的socketfd
                int close_listen_fd();          //关闭socket以及释放
                
        private:
                vector<socket_fd_ptr> listenPort_vec;   //监听段口的port以及socketfd
                string key;             //配置文件所属段
                int listen_port_num;            //监听端口数量

/*----------------------------------------epoll相关--------------------------------------------------------------*/

        private:
                struct epoll_event events[MAX_EVENTS];          //事假列表
                int  mConnection_number;                //连接池长度
                int  epoll_handler;             //epoll句柄
                connection_pty connection_head;         //连接池首地址
                connection_pty  free_connection_head;           //空闲连接池首地址
                int free_mConnection_number;            //空闲链长度

                int  m_worker_connections;              //epoll最大连接数
                
        
        public:
                void ngx_epoll_init();          //epoll初始化函数 ，创建epoll，空闲链等
                connection_pty get_free_connect();
};


#endif