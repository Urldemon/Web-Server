#pragma once
#ifndef _PTHREADPOOL_H_
#define _PTHREADPOOL_H_

#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

#define PTHREAD_DEL_SIZE 2
#define PTHREAD_CRE_SIZE 2

struct task{
    int fd;
    int buf[1024];
    void* (*callbackfun)(void *);
    void* arg;
}; 

class PthreadPool{
public:
    PthreadPool(int copacity,int min_size,int max_queue);
    ~PthreadPool();
    bool appque(task * request);
    // 关闭复制构造和=号复制
    PthreadPool(PthreadPool&) = delete; 
    PthreadPool& operator=(const PthreadPool &)= delete;
private:
    static void * Worder(void *arg);
    static void * Marge_Worder(void *arg);
private:
    // 线程池容量
    int pthread_pool_copacity;
    // 线程最小个数
    int pthread_pool_minsize;
    // 线程池存活线程数
    int pthread_live_size;
    // 工作中线程数
    int pthread_word_size;
    
    // 任务队列
    int queue_size;
    std::queue<task*> work_queue; 

    // 线程池
    pthread_t *m_thread;
    pthread_t *marge_thread;

    // 同步锁
    pthread_mutex_t pool_mutex_lock;  
    pthread_cond_t pool_cond_lock;

    // 线程池状态
    int shutdown;
    bool off;
};
#endif
