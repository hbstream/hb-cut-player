#ifndef _BASE_H_
#define _BASE_H_

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#pragma warning(disable:4005)
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <cstring>

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#define MY_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MY_MIN(a,b) (((a) < (b)) ? (a) : (b))

inline unsigned int CurrentTickCount()
{
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

inline void MillsecSleep(int millsec)
{
#ifdef WIN32
    Sleep(millsec);
#else
    struct timeval t_timeval;
    t_timeval.tv_sec = 0;
    t_timeval.tv_usec = millsec*1000;
    select(0, 0, 0, 0, &t_timeval);
#endif
}

#ifndef WIN32
typedef struct
{
    int wSecond;			 /* Seconds.	 [0-60] (1 leap second) */
    int wMinute;			 /* Minutes.	 [0-59] */
    int wHour;			 /* Hours.	 [0-23] */
    int wDay;			    /* Day.		 [1-31] */
    int wMonth;			 /* Month.	 [0-11] */
    int wYear;			 /* Year	 - 1900.  */
    int wDayOfWeek;		 /* Day of week.	 [0-6] */
    int tm_yday;			 /* Days in year.[0-365]	 */
    int tm_isdst;			 /* DST.		 [-1/0/1]*/
#ifdef	 __USE_BSD
    long int tm_gmtoff;		 /* Seconds east of UTC.  */
    __const char *tm_zone;	 /* Timezone abbreviation.  */
#else
    long int __tm_gmtoff;		 /* Seconds east of UTC.  */
    __const char *__tm_zone;	 /* Timezone abbreviation.  */
#endif
}SYSTEMTIME;

inline void GetLocalTime(SYSTEMTIME *st)
{
    if(st)
    {
        struct tm *pst = NULL;
        time_t t = time(NULL);
        pst = localtime(&t);
        memcpy(st,pst,sizeof(SYSTEMTIME));
        st->wYear += 1900;
        st->wMonth += 1;
        st->wDayOfWeek = (pst->tm_wday+1)%7;
    }
}

#endif // #ifndef WIN32

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);               \
    void operator=(const TypeName&)

#endif  // _BASE_H_
