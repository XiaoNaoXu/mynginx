
#include <sys/socket.h>
#include <arpa/inet.h>
#include "func.h"

int sim_server()
{
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(struct sockaddr_in));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(10000);
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        int ser_socket = socket(AF_INET, SOCK_STREAM, 0);
        if(bind(ser_socket, (struct sockaddr*)&sa, sizeof( struct sockaddr_in)) == -1)
        {
                ngx_log_stderr(6, "bind失败  ");
                return -1;
        }
        if(listen(ser_socket, 50) == -1)
        {
                ngx_log_stderr(6, "listen失败  ");
                return -1;
        }
        int client;
        for(;;)
        {
                client = accept(ser_socket, (struct sockaddr*) NULL, NULL);
                write(client, "hello\n", strlen("hello\n"));
                close(client);
        }
        return 0;
}