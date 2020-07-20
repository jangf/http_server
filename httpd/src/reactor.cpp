#include"../include/reactor.h"
#include<unistd.h>
namespace jfz
{
    reactor::reactor()
    {
        epfd=::epoll_create(1024);
    }

    reactor::~reactor()
    {
        ::close(epfd);
    }

    int reactor::register_in(int fd,__U32_TYPE type)
    {
        struct epoll_event ev;
        std::memset(&ev,0,sizeof(ev));
        ev.events=type;
        ev.data.fd=fd;
        return ::epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    }
    int reactor::register_out(int fd,__U32_TYPE type)
    {
        struct epoll_event ev;
        memset(&ev,0,sizeof(ev));
        ev.events=type;
        ev.data.fd=fd;
        return ::epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
    }

    int reactor::modify(int fd,__U32_TYPE type)
    {
        struct epoll_event ev;
        memset(&ev,0,sizeof(ev));
        ev.events=type;
        ev.data.fd=fd;
        return ::epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
    }
   
    int reactor::wait(struct epoll_event *events)
    {
        const int maxevents=20;
        const int waitms=10;
        return ::epoll_wait(epfd,events,maxevents,waitms);
    }

    int reactor::get_epfd()
    {
        return epfd;
    }

}
