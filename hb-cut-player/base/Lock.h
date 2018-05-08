#ifndef _LOCK_H_
#define _LOCK_H_

#include "base.h"

namespace base{

class Lock
{
public:
    Lock();

    ~Lock();

    bool Try();

    void Acquire();

    void Release();

private:
#ifdef WIN32
    CRITICAL_SECTION mtx_;
#else
    pthread_mutex_t mtx_;
#endif

    DISALLOW_COPY_AND_ASSIGN(Lock);
};

//
// Lock辅助类
// 构造函数-加锁
// 析构函数-释放锁
//
class AutoLock {
public:
    explicit AutoLock(Lock& lock) : lock_(lock) {
        lock_.Acquire();
    }

    ~AutoLock() {
        lock_.Release();
    }

private:
    Lock& lock_;
    DISALLOW_COPY_AND_ASSIGN(AutoLock);
};

//
// Lock辅助类
// 构造函数-释放锁
// 析构函数-加锁
//
class AutoUnlock {
public:
    explicit AutoUnlock(Lock& lock) : lock_(lock) {
        lock_.Release();
    }

    ~AutoUnlock() {
        lock_.Acquire();
    }

private:
    Lock& lock_;
    DISALLOW_COPY_AND_ASSIGN(AutoUnlock);
};

} // namespace Base

#endif // _LOCK_H_
