#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
template <class T>
class Socket{
public:
    Socket(const std::string ip,int16_t port,int listen,bool model);
    int git_sfd();
    int Accept(int fd,T g_events);
    ~Socket();
private:
    int sfd;
    void perr_exit(const std::string str,int ret) ; 
};
#endif

