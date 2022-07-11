# Web-Server
web网络访问服务器

## 工作线程池
+ 线程池对象在文件夹`pthread_pool`上,大致可分为3个部分、任务队列、工作贤臣、管理线程
### 线程池介绍
该线程由工作线程和管理线程组成，线程之间的同步机制为生产者消费模型，通过互斥锁、条件变量组成。该线程池通过queue的结构存储任务，任务类型为`task`的结构体。
通过接口`bool appque(task *request);`加任务添加至任务队列
#### task结构体
+ 结构体如下
    ```c++
        struct task{
            int fd;
            int buf[1024];
            void* (*callbackfun)(void *);
            void* arg;
        };
    ```
+ 结构体中的callbackfun是线程接受到任务后执行的回调函数,其参数为task自身。可以通过task上的arg传入自定义的数据。
#### 工作线程
+ 可以根据自身需求配套搭配工作模式
+ 该项目用的是多线程反应堆模型
    IO获取信息后，分析是否是连接请求，连接请求由主线程实现，读写业务由其余线程实现。回调函数运用task中的callbackfunc实现，可根据自身需求进行设计回调函数。
#### 管理线程
该线程负责管理存货线程的数量管理，当处理的任务较少是则关闭一定量线程节省资源,当任务量多时则增加工作线程
+ 当前存活线程数大于线程池最小线程数且工作线程数*2小于存活线程数时，对线程池中的线程数进行关闭，按照宏定义`PTHREAD_DEL_SIZE`大小进行缩减。
+ 当任务数大于等于存活线程数时,按照宏定义`PTHREAD_CAT_SIZE`大小进行扩增。

