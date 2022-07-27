#include <iostream>
using namespace std;
#include "epoll.h"
int main(){
    Epoll* ptr = Epoll::init_epoll();
    cout << ptr->epfd <<endl; 
    return 0;
}
