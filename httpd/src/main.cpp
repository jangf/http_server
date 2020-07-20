#include"../include/multi_thread.h"

int main()
{
    //::signal(SIGINT,SIG_IGN);
    jfz::http_server serv;
    std::cout<<"******************************"<<std::endl;
    std::cout<<"*  the http server is on     *"<<std::endl;
    std::cout<<"* enter the \"CTRL+C\" to exit *"<<std::endl;
    std::cout<<"******************************"<<std::endl;
    serv.kick_off("0.0.0.0",8080);
    return 0;
}