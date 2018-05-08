#ifndef _TIME_COUNTER_H_
#define _TIME_COUNTER_H_

#include "base/base.h"

#if defined(WIN32)
#include <Mmsystem.h>
#endif

class TimeCounter
{
public:
    TimeCounter()
    {
        Clear();
    }

    long long Now()
    {
#if defined(WIN32)
        unsigned long now = timeGetTime();
        if (now < last_system_now_)
        {
            rollover_millsecs_ += 0x100000000I64;   // ~49.7 days.
        }
        last_system_now_ = now;
        return now + rollover_millsecs_;
#else
        return CurrentTickCount();
#endif
    }

    void Reset()
    {
        last_timestamp_ = Now();
    }

    long long Get()
    {
        return Now() - last_timestamp_;
    }

    long long LastTimestamp()
    {
        return last_timestamp_;
    }

    void Clear()
    {
        last_system_now_ = 0;
        rollover_millsecs_ = 0;
        last_timestamp_ = -1;
    }

private:
    unsigned long last_system_now_;
    long long rollover_millsecs_;
    long long last_timestamp_;
};

#endif // _TIME_COUNTER_H_
