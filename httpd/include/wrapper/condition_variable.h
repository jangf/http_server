#ifndef _CONDITION_VARIABLE_H_
#define _CONDITION_VARIABLE_H_

#include<pthread.h>
#include<iostream>

namespace jfz
{
    class mutex;
    class condition_variable
    {
        pthread_cond_t cond;
    public:
        condition_variable();
        ~condition_variable();
        int wait(mutex&);
        int notify_one();
        int notify_all();
    };
}

#endif