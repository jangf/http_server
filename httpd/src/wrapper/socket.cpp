#include"../../include/wrapper/socket.h"

namespace jfz
{
    listen_socket::listen_socket(::in_port_t port)
    {
        sockfd=::socket(AF_INET, SOCK_STREAM, 0); 
        struct ::sockaddr_in servaddr;
        ::memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_addr.s_addr = ::htonl(INADDR_ANY); //IP地址INADDR_ANY
        servaddr.sin_port = ::htons(port); //端口号
        ::bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));//绑定fd和端口号、IP
        ::listen(sockfd,1024);
     }

    listen_socket::~listen_socket()
    {
        ::close(sockfd);
    }

    int listen_socket::accept()
    {
        return ::accept(sockfd,(struct sockaddr*)nullptr,nullptr);
    }

    int listen_socket::set_nonblock()
    {
        int flags=::fcntl(sockfd, F_GETFL,0);
        return ::fcntl(sockfd,F_SETFL,flags|O_NONBLOCK); //设置为非阻
    }

    int listen_socket::get_fd()
    {
        return sockfd;
    }

    rw_socket::~rw_socket()
    {
        
    }

    int rw_socket::read(char* buf,size_t size)
    {
        return ::read(sockfd,buf,size);
    }

    int rw_socket::write(char* buf,size_t size)
    {
        return ::write(sockfd,buf,size);
    }

    int rw_socket::set_nonblock()
    {
        int flags=::fcntl(sockfd, F_GETFL,0);
        return ::fcntl(sockfd,F_SETFL,flags|O_NONBLOCK);//设置为非阻塞
    }

    int rw_socket::close()
    {
        return ::close(sockfd);
    }

}