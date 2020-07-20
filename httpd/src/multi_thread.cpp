#include"../include/multi_thread.h"
#include<errno.h>
#include"../include/debug/log.h"
namespace jfz
{
    
    http_server::http_server()
    {
        port=0;
        for(int i=0;i<8;++i)thread_id[i]=-1;
        poolptr=nullptr;
    }

    http_server::~http_server()
    {
        delete poolptr;
    }

    int http_server::kick_off(std::string ip_addr,short port_num)
    {
        ip=ip_addr;
        port=port_num;
        poolptr=new ThreadPool(8);
        work_thread_param_t param[8];
        pthread_t main_thread_id,work_thread_id[8];
        ::pthread_create(&main_thread_id,nullptr,main_thread_func,this);
        for(int i=0;i<8;++i)
        {
            param[i].pool=(poolptr);
            param[i].th=&thread[i];
            ::pthread_create(&work_thread_id[i],nullptr,work_thread_func,&param[i]);
        }

        ::pthread_join(main_thread_id,nullptr);
        for(int i=0;i<8;++i)
        {
            ::pthread_join(work_thread_id[i],nullptr);
        }
        return 0;
    }

    void http_server::main_thread_exit(int signum)
    {
        std::cout<<"main thread exit"<<std::endl;
        ::pthread_exit(0);
    }

    void http_server::worker_thread_exit(int signum)
    {
        std::cout<<"worker thread exit"<<std::endl;
       ::pthread_exit(0);//线程退出
    }

    void* http_server::main_thread_func(void *arg)
    {
        //::signal(SIGINT,http_server::main_thread_exit);

        http_server *server=(http_server*)(arg);
        int num=0;
        reactor main_reactor;
        listen_socket listen(server->port);
        listen.set_nonblock();
        int i=main_reactor.register_in(listen.get_fd(),EPOLLIN |EPOLLET);//边沿触发，默认为水平触发
        for(;;)
        { 
            struct epoll_event active_events[100];
            int n=main_reactor.wait(active_events);
            for(int i=0;i<n;++i)
            {
                if(active_events[i].events & (EPOLLIN | EPOLLET))
                {
                    if(active_events[i].data.fd==listen.get_fd())
                    {
                        int fd=listen.accept();
                        rw_socket rw(fd);
                        rw.set_nonblock();
                        std::cout<<"connection from browser"<<std::endl;
                        //dispatch
                        server->thread[num].from_main_thread.push(fd);
                        ++num;
                        num=num%8;
                    }
                }
            }
        }
    }

    void *http_server::work_thread_func(void *arg)
    {
        //::signal(SIGINT,http_server::worker_thread_exit);

        std::cout<<"worker thread id is "<<::pthread_self()<<std::endl;//日志

        work_thread_param_t *param=(work_thread_param_t*)(arg);
        reactor work_reactor;
        for(;;)
        {
            struct epoll_event active_events[100];
            int n=work_reactor.wait(active_events);
            for(int i=0;i<n;++i)
            {
                if(active_events[i].events & (EPOLLIN | EPOLLET))
                {
                    char* buf=new char[4096];
                    //传递给线程池的信息，一定要采用堆分配，然后经过线程池使用后释放
                    //如果采用栈分配，则无限循环会占用大量内存空间
                    struct worker_to_threadpool *to_pool=new struct worker_to_threadpool;
                    std::cout<<"here comes the request http gram:"<<active_events[i].data.fd<<std::endl;
                    
                    //read请求报文到缓冲区                    
                    rw_socket sock(active_events[i].data.fd);
                    int ret=sock.read(buf,4096);

                    exit_if(ret==-1,"read failure");
                    exit_if(ret==EAGAIN,"cache is empty");
                    buf[ret]='\0';//把buf拼接成字符串
                    std::cout<<ret<<'\n'<<buf<<std::endl;
                    //等待fd缓冲区为空
                    ret=work_reactor.modify(active_events[i].data.fd,EPOLLOUT);
                    exit_if(ret==-1,"modify failure");
                    //把fd、tid、缓冲区地址打包交给线程池分析报文数据
                    to_pool->fd=active_events[i].data.fd;
                    to_pool->addr=buf;
                    to_pool->bufsize=sizeof(buf);
                    to_pool->th=param->th;
                    param->pool->enqueue(std::move(threadpool_func),std::move(to_pool));
                }
                if(active_events[i].events & (EPOLLOUT))
                {
                    rw_socket sock(active_events[i].data.fd);
                    //std::cout<<"cache is available"<<std::endl;
                    if(param->th->from_threadpool.is_exit(active_events[i].data.fd))//查找map上是否有fd
                    {
                        std::cout<<"send file to browser"<<std::endl;
                        threadpool_to_worker info=param->th->from_threadpool.find_and_erase(active_events[i].data.fd);
                        std::cout<<info.buf_addr<<std::endl;
                        info.buf_mtx.lock();
                        int ret=sock.write(info.buf_addr+(size_t)(info.offset),maxdata-(size_t)(info.offset));
                        if(ret>0) info.offset+=ret;
                        //如果buf缓冲区内的数据未读完，记录现场，等待下次内核
                        //缓冲区有空闲时再读
                        if(info.offset<maxdata)
                        {
                             param->th->from_threadpool.insert(active_events[i].data.fd,info);//把现场信息保存到map上
                            work_reactor.register_out(active_events[i].data.fd,EPOLLOUT);
                        }
                        info.buf_mtx.unlock();
                        close(active_events[i].data.fd);
                    }
                }
            }
            while(!(param->th->from_main_thread).empty())//如果从主线程传数据过来，则添加至监听fd
            {
                int rw_fd=(param->th->from_main_thread).dequeue();
                if(rw_fd!=0) 
                {
                    int ret=work_reactor.register_in(rw_fd,EPOLLIN);
                    exit_if(ret==-1,"register in error");
                }
            }
        }
    }

    int http_server::threadpool_func(struct worker_to_threadpool *info)
    {
         threadpool_to_worker information;
        response res(info->addr);
        //information信息填充
        information.buf_addr=res.handle();
        //挂接到对应线程map上
        info->th->from_threadpool.insert(info->fd,information);
        //delete info;
        return 0;
    }

}
