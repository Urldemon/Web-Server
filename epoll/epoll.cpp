#include "epoll.h"
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <exception>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <iostream>

// 外部初始化
Epoll* Epoll::epo = nullptr;

Epoll* Epoll::init_epoll(){
    if(epo == nullptr){
        epo = new Epoll;
    }
    return epo;
}
// 上树操作
bool Epoll::events_add(int fd,task * ta){
    // 设置属性
    epoll_event event;
    event.data.ptr = ta;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event) == -1){
        perror("epoll_ctl_add");
        return false;
    }
    return true;
}
// 修改event参数
bool Epoll::events_set(int fd,int OP,task * ta){
    struct epoll_event event;
    event.events = OP;
    event.data.ptr = ta;
    if(epoll_ctl(epfd,EPOLL_CTL_MOD, fd,&event) == -1){
        perror("epoll_ctl_set");
        return false;
    }
    return true;
}
// 下树操作
bool Epoll::events_del(int fd){
    if(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL) == -1){
        perror("epoll_ctl_del");
        return false;
    }
    return true;
}
void Epoll::init_socket(std::string IP, int port, int list_num){
    sfd = socket(AF_INET,SOCK_STREAM,0); 
    if(sfd)
}
int Epoll::get_epfd(){
    return epfd;
}
Epoll::~Epoll(){
    delete epo;
}
Epoll::Epoll(){
    epfd = epoll_create(EPOLL_SIZE);
    if(epfd == -1){
        perror("epoll_create");
        exit(-1);
    }
    events = new epoll_event[EPOLL_EVENTS_SIZE]; 
    if(!events){
        perror("epoll_event new");
        exit(-1);
    }
    events = new epoll_event[EPOLL_EVENTS_SIZE];
    if(!events){
        perror("new events");
        exit(-1);
    }
}

