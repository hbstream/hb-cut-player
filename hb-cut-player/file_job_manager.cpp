/*******************************************************************************
 * file_job_manager.cpp
 * Copyright: (c) 2017 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2018-03-29 11:03 - Created (Haibin Du)
 ******************************************************************************/

#include "file_job_manager.h"

FileJobManager::FileJobManager()
{
    cut_thread_ = nullptr;
    job_index_ = 0;
}

FileJobManager::~FileJobManager()
{

}

int FileJobManager::StartCutJob(const std::string& fromName, const std::string& toName,
    int cutBegSeconds, int cutBegFrame, int cutInterval)
{
    int job_id = job_index_;
    job_index_++;

    cut_thread_ = new FileCutThread(job_id, this, fromName, toName,
        cutBegSeconds, cutBegFrame, cutInterval,
        false, tm());
    cut_thread_->Start();

    return job_id;
}

void FileJobManager::StopJob(int jobId)
{
    if (cut_thread_)
    {
        cut_thread_->Stop();
        delete cut_thread_;
        cut_thread_ = nullptr;
    }
}

// ----------------------------------------------------
// FileCutThread::Observer

void FileJobManager::OnFileCutComplete(int jobId)
{
    emit SigCutComplete(jobId);
}

