#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include<queue>
#include"mutex.h"
#include<iostream>
namespace jfz
{
    template<typename T>
    class safequeue
    {
        std::queue<T> q;
        mutex mtx;
    public:
        bool empty();
        void push(T&);
        T dequeue();
    };

    template<typename T>
    bool safequeue<T>::empty()
    {
        mtx.lock();
		bool temp;
		temp=q.empty();
		mtx.unlock();
		return temp;
    }

    template<typename T>
    void safequeue<T>::push(T &a)
    {
        mtx.lock();
        q.push(a);
        mtx.unlock();
    }
    template<typename T>
    T safequeue<T>::dequeue()
    {
        mtx.lock();
		T temp=q.front();//队列为空时，front返回值是什么
        if(!q.empty())
        {
            q.pop();       
        }
        mtx.unlock();
        return temp;
    }
    
}

#endif
