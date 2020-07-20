#ifndef _SOCKET_H_
#define _SOCKET_H_

#include<sys/socket.h>
#include<unistd.h>
#include<iostream>
#include<cstring>
#include<netinet/in.h>
#include<fcntl.h>
namespace jfz
{
    //监听套接字
    class listen_socket
    {
        int sockfd;
        std::string addr;
        ::in_port_t port;
    public:
        listen_socket(::in_port_t);//socket()
        ~listen_socket();//close()
        
        int accept();
        int set_nonblock();
        int get_fd();
    } ;
    //建立连接后的读写套接字
    class rw_socket
    {
        int sockfd;
    public:
        rw_socket(int fd):sockfd(fd){};
        ~rw_socket();
        int read(char*,size_t);
        int write(char*,size_t);
        int set_nonblock();
        int close();
    };
}

#endif