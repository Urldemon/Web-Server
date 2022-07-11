#include "pthreadpool.h"

PthreadPool::PthreadPool(int que_size,int pool_min,int pool_copacity){
    // 判断参数是否真确
    if(que_size <= 0|| pool_copacity <= 0|| pool_min <= 0)throw std::exception();

    // 初始化参数
    this->pthread_pool_copacity = pool_copacity;
    this->pthread_pool_minsize = pool_min;
    this->pthread_live_size = pool_min;
    this->pthread_word_size = 0;
    this->queue_size = que_size;
    this->shutdown = 0;
    this->off = false;

    // 初始化锁
    pthread_mutex_init(&this->pool_mutex_lock,NULL);
    pthread_cond_init(&this->pool_cond_lock,NULL);

    // 设置线程分离
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    // 创建线程 
    this->m_thread = new pthread_t[this->pthread_pool_minsize];
    if(!this->m_thread)
        throw std::exception();

    for(int i = 0;i < this->pthread_pool_minsize;i++){
        if(pthread_create(this->m_thread + i,&attr,Worder,this) != 0){
            delete [] this->m_thread;
            throw std::exception();
        }
        std::cout << "线程"<< this->m_thread+i <<"创建成功" << std::endl;
    }

    // 创建管理线程
    this->marge_thread = new pthread_t;
    if(pthread_create(this->marge_thread,&attr,Marge_Worder,this) != 0)
    {
        delete this->marge_thread;
        delete [] this->m_thread;
        throw std::exception();
    }
    std::cout <<"线程池创建完毕!" << std::endl;
}

PthreadPool::~PthreadPool(){
    this->off = true;
    while(this->pthread_word_size != 0);
    delete this->marge_thread;
    delete [] this->m_thread;
    pthread_mutex_destroy(&this->pool_mutex_lock);
    pthread_cond_destroy(&this->pool_cond_lock);
    std::cout << "线程池对象清除" << std::endl;
}

bool PthreadPool::appque(task *request){
    pthread_mutex_lock(&this->pool_mutex_lock);
    while(this->work_queue.size() == this->queue_size){
        return false;
    }
    // 添加任务
    work_queue.push(request);

    pthread_mutex_unlock(&this->pool_mutex_lock);
    pthread_cond_broadcast(&this->pool_cond_lock);
    return true;
}

void *PthreadPool::Worder(void *arg){
    PthreadPool * ptr = (PthreadPool *)arg;
    while(!ptr->off){
        pthread_mutex_lock(&ptr->pool_mutex_lock);
        while(ptr->work_queue.size() == 0){
            pthread_cond_wait(&ptr->pool_cond_lock,&ptr->pool_mutex_lock);
            if(ptr->shutdown){
                // 修改关闭数量
                ptr->shutdown--;
                ptr->pthread_live_size--;
                // 释放锁，并唤醒条件锁
                pthread_mutex_unlock(&ptr->pool_mutex_lock);
                pthread_cond_signal(&ptr->pool_cond_lock);
                std::cout << "线程" << pthread_self() <<  "关闭成功,剩余：" << ptr->pthread_live_size << "线程" << std::endl;
                ptr = nullptr;
                pthread_exit((void *)2);
            }
        }
        // 处理事件  
        task *ta = ptr->work_queue.front();
        std::cout << ta << std::endl;
        ptr->work_queue.pop();
        ptr->pthread_word_size++;

        pthread_mutex_unlock(&ptr->pool_mutex_lock);
        pthread_cond_broadcast(&ptr->pool_cond_lock);
       
        // 开始工作 
        ta->callbackfun(ta);
        std::cout << "工作结束" << std::endl;

        // 工作结束更改参数
        pthread_mutex_lock(&ptr->pool_mutex_lock);
        ptr->pthread_word_size--;
        pthread_mutex_unlock(&ptr->pool_mutex_lock);

    }
    std::cout << "工作线程" << pthread_self() <<  "关闭成功!" << std::endl;
    pthread_exit((void*)1);
}
void *PthreadPool::Marge_Worder(void *arg){
    PthreadPool *ptr = (PthreadPool *)arg;
    int min_size = ptr->pthread_pool_minsize;
    int max_size = ptr->pthread_pool_copacity;
    while(!ptr->off){
        // 利用锁获取数据
        pthread_mutex_lock(&ptr->pool_mutex_lock);
        int work_size = ptr->pthread_word_size;
        int live_size = ptr->pthread_live_size;
        int que_size = ptr->work_queue.size();
        // 释放锁
        pthread_mutex_unlock(&ptr->pool_mutex_lock);

        // 当工作线程数大于最小线程数，切工作线程数小于存活线程数,关闭部分线程
        if((live_size > min_size)&&(work_size < live_size / 2)){
            pthread_mutex_lock(&ptr->pool_mutex_lock); 
            ptr->shutdown += PTHREAD_DEL_SIZE;
            pthread_mutex_unlock(&ptr->pool_mutex_lock);
            pthread_cond_signal(&ptr->pool_cond_lock);
        }
        // 当工作线程数大于等于生存线程输的
        if(que_size > live_size  && work_size <= live_size){
            // 设置线程分离
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
            // 获取锁
            pthread_mutex_lock(&ptr->pool_mutex_lock); 
            // 每次创建指定线程数量
            for(int i = 0;(i < PTHREAD_DEL_SIZE) && (ptr->pthread_live_size < ptr->pthread_pool_copacity);i++){
                // 创建线程
                pthread_create(ptr->m_thread + live_size + i, &attr,Worder,ptr);
                std::cout <<"线程:" << ptr->m_thread + live_size +i << ",创建成功:" << ptr->pthread_live_size  << std::endl;
                ptr->pthread_live_size++;
            }
            // 释放锁
            pthread_mutex_unlock(&ptr->pool_mutex_lock);
        }
        sleep(10);
        std::cout << ptr->pthread_live_size << "," << ptr->pthread_word_size << "," << ptr->work_queue.size() << std::endl;
    }
    std::cout << "管理线程" << pthread_self() <<  "关闭成功!" << std::endl;
    pthread_exit((void *)1);
}
