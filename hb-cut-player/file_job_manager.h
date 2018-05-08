/*******************************************************************************
 * file_job_manager.h
 * Copyright: (c) 2017 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * 
 *
 * -----------------------------------------------------------------------------
 * 2018-03-29 11:03 - Created (Haibin Du)
 ******************************************************************************/
 
#ifndef _HDEV_FILE_JOB_MANAGER_H_
#define _HDEV_FILE_JOB_MANAGER_H_

#include <QObject>
#include <QThread>

#include "file_cut_thread.h"

class FileJobManager : public  FileCutThread::Observer
{
    

public:
    FileJobManager();

    ~FileJobManager();

    int StartCutJob(const std::string& fromName, const std::string& toName,
        int cutBegSeconds, int cutBegFrame, int cutInterval);

    void StopJob(int jobId);

    // ----------------------------------------------------
    // FileCutThread::Observer

    virtual void OnFileCutComplete(int jobId) = 0;

private:
    FileCutThread* cut_thread_;
    int job_index_;
};

#endif // _HDEV_FILE_JOB_MANAGER_H_