//#include "stdafx.h"
#include "SimpleThread.h"

#ifdef WIN32
#include <process.h>
#endif

namespace base {

THREAD_FUNC_TYPE ThreadFunc(void* args)
{
    SimpleThread* thread = static_cast<SimpleThread*>(args);
    thread->ThreadMain();
    THREAD_FUNC_RETURN();
}

SimpleThread::SimpleThread()
    : is_stop_(true)
{

}

SimpleThread::~SimpleThread()
{

}

void SimpleThread::ThreadStart()
{
#ifdef WIN32
    thread_id_ = (thread_handle_t)::_beginthread(ThreadFunc, 0, this);
#else
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    // 默认是PTHREAD_CREATE_JOINABLE，可以调用pthread_join
    //pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread_id_, &attributes, ThreadFunc, this);
#endif
}

void SimpleThread::ThreadStop()
{
    is_stop_ = true;
}

bool SimpleThread::IsThreadStop()
{
    return is_stop_;
}

void SimpleThread::ThreadJoin()
{
#ifdef WIN32
    ::WaitForSingleObject(thread_id_, INFINITE);
#else
    pthread_join(thread_id_, 0);
#endif
}

void SimpleThread::ThreadMain()
{
    is_stop_ = false;
    ThreadRun();
}

} // namespace base
