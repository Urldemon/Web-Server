#include "z.h"
#include <iostream>
#include <pthread.h>

template<class T>
a<T>::a(int q){
    this->z = q;
    std::cout << "yes" << std::endl;
    pthread_attr_t att;
    pthread_attr_init(&att);
    pthread_attr_setdetachstate(&att,PTHREAD_CREATE_DETACHED);

    this->mthread = new pthread_t[3];
    for(int i = 0;i < 3;i++){
        pthread_create(this->mthread + i,&att,func,(void *)1);
    }

    pthread_mutex_init(&this->mut, NULL);
    pthread_cond_init(&this->cond,NULL);
}
template<class T>
a<T>::~a(){
    std::cout << "end" << std::endl;
    pthread_mutex_destroy(&this->mut);
    pthread_cond_destroy(&this->cond);
}


template<class T>
void * a<T>::func(void *arg){
    std::cout << (int)(long)arg << std::endl;
    pthread_exit(NULL);
}
