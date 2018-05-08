/*******************************************************************************
 * simple_logger.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2015-7-5 14:17 - Created (Haibin Du)
 ******************************************************************************/
#include "base/base.h"
#include "simple_logger.h"

#define HDEV_USE_SIMPLE_LOG 1

SimpleLogger gSimpleLogger;

SimpleLogger::SimpleLogger()
{
#if defined(HDEV_USE_SIMPLE_LOG)
    file_ = NULL;
#endif
}

SimpleLogger::~SimpleLogger()
{
#if defined(HDEV_USE_SIMPLE_LOG)
    Close();
#endif
}

void SimpleLogger::Open(const char* logFilename)
{
#if defined(HDEV_USE_SIMPLE_LOG)
    file_ = fopen(logFilename, "w");
#endif
}

void SimpleLogger::Open(const std::string& logFilename)
{
#if defined(HDEV_USE_SIMPLE_LOG)
    Open(logFilename.c_str());
#endif
}

void SimpleLogger::OpenAppend(const std::string& logFilename)
{
#if defined(HDEV_USE_SIMPLE_LOG)
    Close();
    file_ = fopen(logFilename.c_str(), "a+");
#endif
}

void SimpleLogger::Close()
{
#if defined(HDEV_USE_SIMPLE_LOG)
    if (file_)
    {
        fclose(file_);
        file_ = NULL;
    }
#endif
}

void SimpleLogger::Log(const char* fmt, ...)
{
#if defined(HDEV_USE_SIMPLE_LOG)
    if (!file_) return;

    char msg_str[2048];
    va_list ap;
    va_start(ap, fmt);

    vsnprintf(msg_str, sizeof(msg_str), fmt, ap);

    va_end(ap);

    char time_buf[1024];

    SYSTEMTIME time;
    ::GetLocalTime(&time);

#if defined(WIN32)
    sprintf_s(time_buf, 1024,
        "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        time.wYear,time.wMonth,time.wDay,
        time.wHour,time.wMinute,time.wSecond, time.wMilliseconds
        );
#else
    timeval tv;
    gettimeofday(&tv, 0);
    sprintf(time_buf,
        "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        time.wYear,time.wMonth,time.wDay,
        time.wHour,time.wMinute,time.wSecond, tv.tv_usec/1000
        );
#endif

    char log_str[3072];
#if defined(WIN32)
    sprintf_s(log_str, 3072, "[%s] %s", time_buf, msg_str);
#else
    sprintf(log_str, "[%s] %s", time_buf, msg_str);
#endif

    fprintf(file_, "%s", log_str);
    fflush(file_);
#endif
}

