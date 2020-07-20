#include"../../include/wrapper/mutex.h"

namespace jfz
{
    mutex::mutex()
    {
        ::pthread_mutex_init(&mtx,NULL);
    }
    mutex::~mutex()
    {
        ::pthread_mutex_destroy(&mtx);
    }

    bool mutex::lock()
    {
        return ::pthread_mutex_lock(&mtx);
    }

    bool mutex::unlock()
    {
        return ::pthread_mutex_unlock(&mtx);
    }
}