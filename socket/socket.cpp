#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
#include "socket.h"

template <class T> 
Socket<T>::Socket(const std::string ip,int16_t port,int lis_size,bool model){
    // 获取socket句柄
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd == -1){
        perr_exit("socket",-1);
    }
    // 设置端口复用
    int opt = 1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    // 绑定端口
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int ret = inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);
    if(ret == -1)perr_exit("inet_pton",-1);

    ret = bind(sfd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret == -1)perr_exit("bind",-1);

    // 设置监听个数
    ret = listen(sfd,lis_size);
    if(ret == -1)perr_exit("listen",-1);

}
template<class T>
Socket<T>::~Socket(){
    close(sfd);
}
template<class T>
int Socket<T>::git_sfd(){
    return sfd;
}
template<class T>
void Socket<T>::perr_exit(const std::string str,int ret){
    perror(str.c_str()); 
    exit(ret);
}
