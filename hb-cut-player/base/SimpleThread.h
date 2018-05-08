#ifndef _SIMPLE_THREAD_H_
#define _SIMPLE_THREAD_H_

#include "base.h"

#ifdef WIN32
#define THREAD_FUNC_TYPE void
#define THREAD_FUNC_RETURN()
typedef HANDLE thread_handle_t;
#else
#define THREAD_FUNC_TYPE void*
#define THREAD_FUNC_RETURN() return 0
typedef pthread_t thread_handle_t;
#endif

namespace base
{

class SimpleThread
{
public:
    SimpleThread();

    virtual ~SimpleThread();

    // 启动线程, 保证Start()函数结束时，线程已经开始执行
    virtual void ThreadStart();

    virtual void ThreadStop();

    virtual bool IsThreadStop();

    // 等待线程关闭
    virtual void ThreadJoin();

    // 子类需要重载此函数
    // Run()函数运行在新建的线程内
    virtual void ThreadRun() = 0;

    thread_handle_t ThreadId() { return thread_id_; }

    void ThreadMain();

private:
    thread_handle_t thread_id_;
    volatile bool is_stop_;
};

} // namespace Thread

#endif // _SIMPLE_THREAD_H_
