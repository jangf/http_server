#include"../../include/wrapper/condition_variable.h"
#include"../../include/wrapper/mutex.h"
namespace jfz
{
    condition_variable::condition_variable()
    {
        ::pthread_cond_init(&cond,NULL);
    }
    condition_variable::~condition_variable()
    {
        ::pthread_cond_destroy(&cond);
    }
    int condition_variable::wait(mutex& m)
    {
        pthread_cond_wait(&cond,&(m.mtx));
    }
    int condition_variable::notify_one()
    {
        ::pthread_cond_signal(&cond);
    }
    int condition_variable::notify_all()
    {
        ::pthread_cond_broadcast(&cond);
    }

}