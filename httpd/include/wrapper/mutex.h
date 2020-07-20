#ifndef _MUTEX_H_
#define _MUTEX_H_

#include<pthread.h>
namespace jfz
{
    class mutex
    { 
        ::pthread_mutex_t mtx;
    public:
        mutex();
        ~mutex();
        bool lock();
        bool unlock();
    };
}

#endif

