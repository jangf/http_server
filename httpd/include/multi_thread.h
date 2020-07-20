#ifndef _MULTI_THREAD_H_
#define _MULTI_THREAD_H_

#include"reactor.h"
#include"wrapper/safequeue.h"
#include"wrapper/socket.h"
#include"wrapper/safemap.h"
#include"threadpool.h"
#include"http_gram.h"
#include<cstdlib>
#include<signal.h>

namespace jfz
{

//线程池传递给工作线程的信息，挂接到工作线程结构体上
typedef struct threadpool_to_worker
{
    mutex buf_mtx;
    char* buf_addr;//存放从磁盘读取的数据的缓冲区地址
    size_t bufsize;
    int offset;
    threadpool_to_worker():buf_addr(nullptr),bufsize(0),offset(0){};
    ~threadpool_to_worker(){};
} info_t;

//线程结构体
typedef struct
{
    safequeue<int>  from_main_thread;//来自主线程的信息
    safemap<int,info_t> from_threadpool;//来自线程池的信息
} thread_t;

typedef struct main_to_worker
{
    thread_t *th;
    ThreadPool *pool;
} work_thread_param_t;

//工作线程传递给线程池的信息,以形参的形式传递给线程池
struct worker_to_threadpool
{
    mutex buf_mtx;
    thread_t *th;//该线程的信息
    char* addr;//存放数据的缓冲区地址
    size_t bufsize;
    int fd;
    worker_to_threadpool():th(nullptr),addr(nullptr),bufsize(0),fd(-1){};
    ~worker_to_threadpool(){delete addr;};
};


class http_server
{
public:
    http_server();
    ~http_server();

    static void *main_thread_func(void*);
    static void *work_thread_func(void*);
    static int threadpool_func(struct worker_to_threadpool*);
    //注册用的信号处理函数，退出时清理资源
    static void main_thread_exit(int);
    static void worker_thread_exit(int);

    int kick_off(std::string ip,short port);
    int terminate();
private:
    std::string ip;
    short port;
    int thread_id[8];
    ThreadPool *poolptr;
    thread_t thread[8];
};

}

#endif