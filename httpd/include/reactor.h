#ifndef _REACTOR_H_
#define _REACTOR_H_

#include<sys/epoll.h>
#include<cstring>
namespace jfz
{

class reactor
{
 protected:   
    int epfd;
public:
    reactor();
    ~reactor();
    int register_in(int,__U32_TYPE);
    int register_out(int,__U32_TYPE);
    int modify(int,__U32_TYPE);
    int wait(struct epoll_event*);
    int get_epfd();
};

}

#endif