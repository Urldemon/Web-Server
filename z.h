#define _A_H_
#ifdef _A_H_

#include <pthread.h>

template<class T>
class a{
public:
    a() = default;
    a(int w);
    ~a();
private:
    pthread_mutex_t mut;
    pthread_cond_t cond;
    pthread_t *mthread;
    static void* func(void * arg);
    int z;
};
#endif
