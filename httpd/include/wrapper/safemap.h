#ifndef _SAFEMAP_H_
#define _SAFEMAP_H_

#include<map>
#include"mutex.h"
namespace jfz
{
    template<typename T1,typename T2>
    class safemap
    {
        std::map<T1,T2> mp;
        mutex mtx;
    public:
        //注意一定要有typename，否则编译器不知道std::map<T1,T2>::iterator是一种类型
        using iterator=typename std::map<T1,T2>::iterator;
        iterator begin();
        iterator end();
        void insert(T1,T2);
        bool empty();
        T2 find_and_erase(T1);
        bool is_exit(T1);
    };  
}

namespace jfz
{
    template<typename T1,typename T2>
    typename safemap<T1,T2>::iterator safemap<T1,T2>::begin()
    {
        mtx.lock();
        iterator tmp=mp.begin();
        mtx.unlock();
        return tmp;
    }
    //注意safemap<T1,T2>::iterator前加typename
    template<typename T1,typename T2>
    typename safemap<T1,T2>::iterator safemap<T1,T2>::end()
    {
        mtx.lock();
        iterator tmp=mp.end();
        mtx.unlock();
        return tmp;
    }

    template<typename   T1,typename T2>
    void safemap<T1,T2>::insert(T1 key,T2 value)
    {
        mtx.lock();
        mp.insert(std::pair<T1,T2>(key,value));//std::pair即pair属于名称空间std
        mtx.unlock();
    }

    template<typename T1,typename T2>
    bool safemap<T1,T2>::empty()
    {
        mtx.lock();
        bool flag=mp.empty();
        mtx.unlock();
        return flag;
    }

    template<typename T1,typename T2>
    T2 safemap<T1,T2>::find_and_erase(T1 a)
    {
        mtx.lock();
        iterator iter=mp.find(a);
        mp.erase(iter);
        mtx.unlock();
        return iter->second;
    }

     template<typename   T1,typename T2>
    bool safemap<T1,T2>::is_exit(T1 key)
    {
        mtx.lock();
        bool tmp;
        iterator iter=mp.find(key);
        if(iter==mp.end()) tmp=false;
        else tmp=true;
        mtx.unlock();
        return tmp;
    }

}

#endif