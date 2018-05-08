/*******************************************************************************
 * simple_logger.h
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 *
 *
 * -----------------------------------------------------------------------------
 * 2015-7-5 14:17 - Created (Haibin Du)
 ******************************************************************************/

#ifndef _HDEV_SIMPLE_LOGGER_H_
#define _HDEV_SIMPLE_LOGGER_H_

#include <cstdarg>
#include <cstdio>
#include <deque>
#include <string>

//#define HDEV_USE_SIMPLE_LOG 1

class SimpleLogger
{
public:
    SimpleLogger();

    ~SimpleLogger();

    void Open(const char* logFilename);

    void Open(const std::string& logFilename);

    void OpenAppend(const std::string& logFilename);

    void Close();

    void Log(const char* fmt, ...);

private:
    FILE* file_;
};

extern SimpleLogger gSimpleLogger;

//#if defined(HDEV_USE_SIMPLE_LOG)
    #define SIMPLE_LOG_INIT(filename)   gSimpleLogger.Open(filename)
    #define SIMPLE_LOG_APPEND_INIT(filename)   gSimpleLogger.OpenAppend(filename)
    #define SIMPLE_LOG(fmt, ...)        gSimpleLogger.Log("%s(%d) "fmt, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define SIMPLE_LOG_CLOSE()          gSimpleLogger.Close();
// #else
//     #define SIMPLE_LOG_INIT(filename)
//     #define SIMPLE_LOG(fmt, ...)
//     #define SIMPLE_LOG_CLOSE()
// #endif

#endif // _HDEV_SIMPLE_LOGGER_H_
