/*******************************************************************************
 * file_cut_thread.h
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * 文件剪切线程
 *
 * -----------------------------------------------------------------------------
 * 2016-12-2 23:01 - Created (Haibin Du)
 ******************************************************************************/

#ifndef _HDEV_FILE_CUT_THREAD_H_
#define _HDEV_FILE_CUT_THREAD_H_

#include "base/SimpleThread.h"

#include <string>

class FileCutThread : public base::SimpleThread
{
public:
    class Observer
    {
    public:
        virtual ~Observer() {}

        virtual void OnFileCutComplete(int cutId) = 0;
    };

    static int Execute(const char* cmd_line, bool isWait,
        bool isNeedConsole = false);

    static bool ExecMediaInfoCheckHasVideo(const std::string& mediaName);

public:
    // cutInterval为0， 表示截图
    // cutInterval为-1，表示剪切整个后半段
    explicit FileCutThread(int cutId, Observer* ob,
        const std::string& fromName, const std::string& toName,
        int cutBegSeconds, int cutBegFrame, int cutInterval,
        bool isPicList, const std::tm& fileBegDate);

    ~FileCutThread();

    void Start();

    void Stop();

    void SetObserver(Observer* ob);

    void SetSecFrameCount(int secFrames);

    const std::string& FromName() { return from_name_; }

    virtual void ThreadRun();

private:
#if 0
    void ProcessPicListNames(const std::string& blackPath);
#endif

private:
    int cut_id_;
    Observer* observer_;

    std::string from_name_;
    std::string to_name_;

    int cut_beg_second_;
    int cut_beg_frame_;
    int cut_interval_;

    bool is_pic_list_;
    std::tm file_beg_date_;
    int sec_frame_count_;
};

#endif // _HDEV_FILE_CUT_THREAD_H_
