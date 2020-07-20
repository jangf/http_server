#ifndef _HTTP_GRAM_H_
#define _HTTP_GRAM_H_
#include<iostream>
#include<cstring>
#include<map>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<cstdlib>
namespace jfz
{

#define ROOT "/home/jiangfangzhou/cpplearning/httpd/Html"
const int maxdata=1048576;//最大支持的文本传输大小

typedef std::map<std::string, std::string> http_head_t;

//请求报文解析器
class request
{
protected:
    //解析前：存放报文的缓冲区
    char *gram;
    size_t gram_length;
    //解析后：把收到的请求报文格式的信息经过parse存放在这里
    //请求行
    std::string method; //请求方法：本服务器用不到
    std::string url;//URL
    std::string version;//协议版本：本服务器用不到
    //请求报文头部
    http_head_t header;//一个主要功能就是说明body类型：html/css/gif/png...
    //请求体
    std::string body;
 
    std::string file_path;
    std::string extension_name;
    std::string file_type;
    std::string request_parameter;

    //公有成员函数使用到的数据结构、被调用函数
public:    
    request(char*);
    ~request();
};

class response: public request
{
protected:
    //封装前：组装响应报文需要的信息
    //状态行
    std::string version; //协议版本
    std::string status;     //状态码
    std::string desc;       //状态码描述符
    //响应报文头部
    std::string content_length; //请求头部由关键字/值对组成，每行一对
    std::string connection;
    std::string content_type;
    //响应正文
    //封装好前：存放响应报文数据部分的缓冲区
    char* response_gram;
    char *GET_handle();
    char *POST_handle();
public:
    response(char*);
    ~response();
    char *handle();//把响应报文的信息按顺序填入缓冲区，组装成HTTP报文
};

}

#endif