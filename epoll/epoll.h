#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>
#include <iostream>
#include <string>

#include "../pthread_pool/pthreadpool.h"
#define EPOLL_SIZE 10
#define EPOLL_EVENTS_SIZE 1024
class Epoll{
public:
    bool events_add(int fd,task *ta);
    bool events_set(int fd,int OP,task *ta);
    bool events_del(int fd);
    int events_wait();
    int get_epfd();
    void init_socket(std::string IP,int port,int list_num);
    static Epoll * init_epoll();
private:
    Epoll();
    ~Epoll();
    static Epoll *epo;
    epoll_event *events; 
    
    int epfd;
};
#endif
