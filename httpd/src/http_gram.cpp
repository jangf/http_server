#include"../include/http_gram.h"

namespace jfz
{
    request::request(char *buf)
    {
        int idx=0,tmp=0;
        gram=buf;
        gram_length=::strlen(buf);//获取报文长度，注意不能用sizeof

        //提取请求行信息
        //获得method信息
        for(idx=tmp;idx<gram_length;++idx)
        {
            if(gram[idx]==' ')
            {
                method.assign(gram,idx-tmp);
                tmp=idx+1;
                break;
            }
        }
        
        //获得url信息
        for(idx=tmp;idx<gram_length;++idx)
        {
            if(gram[idx]==' '||gram[idx]=='?')
            {
                url.assign(&gram[tmp],idx-tmp);
                tmp=idx+1;
                break;
            }
        }
        //如果是带有参数的GET请求报文，获得参数信息
        if(gram[idx]=='?')
        {
            for(idx=tmp;idx<gram_length;++idx)
            {
                if(gram[idx]==' ')
                {
                    request_parameter.assign(&gram[tmp],idx-tmp);
                    tmp=idx+1;
                    break;
                }
            }
        }
        //获得version信息
        for(idx=tmp;idx<gram_length;++idx)
        {
            if(gram[idx]=='\r'&&gram[idx+1]=='\n')
            {
                version.assign(&gram[tmp],idx-tmp);
                tmp=idx+2;
                break;
            }
        }
        //获得头部信息，请求头部由键值对组成，关键字和值之间用"："分隔。
        //本服务器不需要头部信息
        for(idx=tmp;idx<gram_length;++idx)
        {
            if(gram[idx]=='\r'&&gram[idx+1]=='\n'&&gram[idx+2]=='\r'&&gram[idx+3]=='\n')
            {
                tmp=idx+4;
                break;
            }
        }
        //获得请求体信息
        idx=tmp;
        body.assign(&gram[idx],gram_length-tmp);

        //根据url获得路径信息
        std::string path(ROOT);
        file_path.assign(path);
        file_path.append(url);
        if(strcmp(url.c_str(),"/")==0)
        {
            std::string filename("/index.html");
            file_path.append(filename);  
        }
        std::cout<<file_path<<std::endl;
        //根据url获取请求文件类型即文件后缀名
        int index=file_path.find_last_of('.');
        extension_name.assign(file_path,index+1,file_path.size()-index);

        //根据文件名后缀获得文件类型
        if(strcmp(extension_name.c_str(),"html")==0)
            file_type.assign("text/html");
        else if(strcmp(extension_name.c_str(),"css")==0)
            file_type.assign("text/css");
        else if(strcmp(extension_name.c_str(),"gif")==0)
            file_type.assign("image/gif");
        else if(strcmp(extension_name.c_str(),"png")==0)
            file_type.assign("image/png");
        else if(strcmp(extension_name.c_str(),"jpeg")==0)
            file_type.assign("image/jpeg");

        //根据method用不同方法解析请求报文数据部分
        //GET无数据部分，POST数据部分存放参数，以名称/值的形式出现

    }

    request::~request()
    {
        delete gram;
    }

    //利用：构造函数的执行顺序为先执行父类再执行子类
    response::response(char* buf):request(buf)
    {
        version=request::version;
    }

    response::~response()
    {
        
    }

    char *response::handle()
    {
        char  _method[method.size()];
        ::strcpy(_method,method.c_str());
        if(strcmp(_method,"GET")==0)
        {
            return GET_handle();
        }
        //else if(strcmp(_method,"POST")==0)
        //{
           // return POST_handle();
        //}
    }

    char *response::GET_handle()
    {
        //根据文件是否可读、可否找得到，初始化响应报文首部行状态码、头部
        struct stat file_info;
        FILE *fp=::fopen(request::file_path.c_str(),"rb");//注意：要以二进制的形式打开文件
        //读错误
        if(fp==nullptr)
        {
            char res[]="HTTP/1.1 404 Not Found\r\nConnection:close\r\nContent-Type:text/html\r\nContent-Length:0\r\n\r\n";
            response_gram=new char[sizeof(res)];
            ::strcpy(response_gram,res);
        }
        //若读写正常：
        //根据文件大小初始化头部Content-Length信息
        //根据请求报文文件名后缀初始化头部Content-Type:html/css
        //初始化Connection:close/keep-alive
        else
        {
           response_gram=new char[maxdata];//1MB大小
           ::memset(response_gram,0,sizeof(response_gram));
           ::stat(file_path.c_str(),&file_info);
           //通过sprintf把描述文件大小的int型数据转化为字符串
            ::sprintf(response_gram,"%s 200 OK\r\nConnection:close\r\nContent-Type:%s\r\nContent-Length:%ld\r\n\r\n",version.c_str(),file_type.c_str(),file_info.st_size);
            size_t offset=::strlen(response_gram);
            //要求文件不能过大，要小于4096字节，从而保证一次读出所有文件到缓冲区
            //！错误：size_t n=read(fd,response_gram+offset-1,sizeof(response_gram)-offset-1); 
            //char buffer[40960];
            int ret=::fread(response_gram+offset,1,file_info.st_size,fp);//二进制文件单个字符1位
            //std::cout<<"read num is "<<ret<<std::endl;
            //::strcpy(response_gram+offset,buffer);
            //std::cout<<"read num is "<<strlen(buffer)<<std::endl;
            //std::cout<<buffer<<std::endl;
        }
        return response_gram;
    }

    
}

