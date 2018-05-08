/*******************************************************************************
 * file_cut_thread.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2016-12-2 23:09 - Created (Haibin Du)
 ******************************************************************************/

#include "file_cut_thread.h"

#include <Tlhelp32.h>

#include <ctime>
#include <fstream>
#include <sstream>

#include "base/file_helper.h"
#include "base/str_helper.h"

//#include "dir_file_cutting.h"

#define DEFAULT_SEC_FRAME_COUNT 25

FileCutThread::FileCutThread(int cutId, Observer* ob,
    const std::string& fromName, const std::string& toName,
    int cutBegSeconds, int cutBegFrame, int cutInterval,
    bool isPicList, const std::tm& fileBegDate)
    : from_name_(fromName), to_name_(toName)
{
    cut_id_ = cutId;
    observer_ = ob;

    cut_beg_second_ = cutBegSeconds;
    cut_beg_frame_ = cutBegFrame;
    cut_interval_ = cutInterval;

    is_pic_list_ = isPicList;
    file_beg_date_ = fileBegDate;

    sec_frame_count_ = DEFAULT_SEC_FRAME_COUNT;
}

FileCutThread::~FileCutThread()
{

}

void FileCutThread::Start()
{
    base::SimpleThread::ThreadStart();
}

void FileCutThread::Stop()
{
    base::SimpleThread::ThreadStop();

    // 

    base::SimpleThread::ThreadJoin();
}

void FileCutThread::SetObserver(Observer* ob)
{
    observer_ = ob;
}

void FileCutThread::SetSecFrameCount(int secFrames)
{
    sec_frame_count_ = secFrames;
}

void FileCutThread::ThreadRun()
{
    std::string ffmpegpath = filehelper::ThisProcessDir() + "tools\\ffmpeg.exe";

    std::ostringstream oss;

    double beg_frame_seconds = cut_beg_second_;
    if (cut_beg_frame_ >= 0)
    {
        beg_frame_seconds += (cut_beg_frame_ * 1.0) / 25;
    }

    if (cut_interval_ != 0)    // 剪切视频段
    {
        if (is_pic_list_)
        {
            bool is_has_video = ExecMediaInfoCheckHasVideo(from_name_);
            if (is_has_video)
            {
                oss << " -i \"" << from_name_ << "\"";
                oss << " -ss " << cut_beg_second_ << " -t " << cut_interval_;
                oss << " -r " << sec_frame_count_ << " -f image2";
                oss << " -y \"" << to_name_ << "%d.jpg\"";

                std::string avi_cmd = oss.str();
                int exeret = Execute((ffmpegpath + avi_cmd).c_str(), true);
#if 0
                ProcessPicListNames("");
#endif
            }
            else
            {
                std::string black_path = filehelper::ThisProcessDir() + "black.jpg";
#if 0
                ProcessPicListNames(black_path);
#endif
            }
        }
        else
        {
            //oss << " -ss " << cut_beg_second_ << " -t " << cut_interval_;
            oss << " -i \"" << from_name_ << "\"";
            oss << " -ss " << cut_beg_second_ << " -t " << cut_interval_;
            oss << " -vcodec copy";
            oss << " -acodec copy";
            oss << " -y \"" << to_name_ << "\"";

            std::string avi_cmd = oss.str();
            int exeret = Execute((ffmpegpath + avi_cmd).c_str(), true);
        }
    }
    else    // 截图或波形
    {
        bool is_take_picture = ExecMediaInfoCheckHasVideo(from_name_);

        if (is_take_picture)
        {
            oss << " -ss " << beg_frame_seconds;
            oss << " -i \"" << from_name_ << "\"";
            oss << " -f image2";
            oss << " -y \"" << to_name_ << "\"";

            std::string avi_cmd = oss.str();
            int exeret = Execute((ffmpegpath + avi_cmd).c_str(), true);
        }
        else
        {
            // 输出临时wav文件

            std::string wavname = to_name_ + ".wav";

            double sec_of_frame = 1.0 / 25;

            oss << " -ss " << beg_frame_seconds << " -t " << sec_of_frame;
            oss << " -i \"" << from_name_ << "\"";
            oss << " -vn -acodec pcm_s16le -ar 44100 -ac 1";
            oss << " -y \"" << wavname << "\"";

            std::string audio_cmd = oss.str();
            int exeret = Execute((ffmpegpath + audio_cmd).c_str(), true);

            // 读取wav文件，并绘图保存
            //WavPicDraw picdraw;
            //picdraw.Draw(wavname, strhelper::Ansi2Wide(to_name_));

            // 删除临时wav文件
            std::remove(wavname.c_str());
        }
    }

    ThreadStop();

    if (observer_)
    {
        observer_->OnFileCutComplete(cut_id_);
    }
}
#if 0
void FileCutThread::ProcessPicListNames(const std::string& blackPath)
{
    std::string dir_path = to_name_;

    std::string channel_name;
    DirFileCutting::ParseFilenameInfo(from_name_, &channel_name, NULL, NULL);

    std::time_t tmp_time = std::mktime(&file_beg_date_);

    CTime begtime(tmp_time);
    CTimeSpan plustime(0, 0, 0, cut_beg_second_);
    begtime += plustime;

    int pic_count = cut_interval_ * sec_frame_count_;
    int frame_n = 0;
    for (int i = 0; i < pic_count; ++i)
    {
        frame_n++;

        char namebuf[1024];
        if (sec_frame_count_ > 1)
        {
            sprintf(namebuf, "%s@%4d-%02d-%02d-%02d-%02d-%02d@%d.jpg",
                channel_name.c_str(),
                begtime.GetYear(), begtime.GetMonth(), begtime.GetDay(),
                begtime.GetHour(), begtime.GetMinute(), begtime.GetSecond(),
                frame_n);
        }
        else
        {
            sprintf(namebuf, "%s@%4d-%02d-%02d-%02d-%02d-%02d.jpg",
                channel_name.c_str(),
                begtime.GetYear(), begtime.GetMonth(), begtime.GetDay(),
                begtime.GetHour(), begtime.GetMinute(), begtime.GetSecond());
        }

        std::string new_pic_name = dir_path + namebuf;

        std::string old_pic_name;
        if (blackPath.empty())
        {
            std::ostringstream oss;
            oss << to_name_ << i + 1 << ".jpg";

            old_pic_name = oss.str();

            std::rename(old_pic_name.c_str(), new_pic_name.c_str());
        }
        else
        {
            ::CopyFileA(blackPath.c_str(), new_pic_name.c_str(), FALSE);
        }

        if (frame_n == sec_frame_count_)
        {
            frame_n = 0;

            CTimeSpan sectime(0, 0, 0, 1);
            begtime += sectime;
        }
    }
}
#endif
static void GetInfoKey(const std::string& str, std::string& keyStr, std::string& valStr)
{
    int pos = str.find(":");
    if (pos >= 0)
    {
        keyStr = str.substr(0, pos);
        strhelper::StringTrim(keyStr);
        valStr = str.substr(pos + 1);
        strhelper::StringTrim(valStr);
    }
    else
    {
        keyStr = strhelper::StringTrimCopy(str);
    }
}

// static
bool FileCutThread::ExecMediaInfoCheckHasVideo(const std::string& mediaName)
{
    std::string tmpfile = mediaName + "_tmp.txt";

    std::string mediacmd = filehelper::ThisProcessDir() + "tools\\MediaInfo.exe";

    std::ostringstream oss;
    oss << mediacmd << " --logfile=\"" << tmpfile << "\" --full \"" << mediaName << "\"";

    int ret = Execute(oss.str().c_str(), true);

    int video_duration = 0;

    std::ifstream file(tmpfile);
    if (file.is_open())
    {
        bool is_video = false;
        bool is_audio = false;
        while (1)
        {
            if (file.eof()) break;

            char linebuf[4096];
            file.getline(linebuf, 4096);
            if (linebuf[strlen(linebuf) - 1] == '\n')
            {
                linebuf[strlen(linebuf) - 1] = '\0';
            }

            std::string keystr, valstr;
            GetInfoKey(linebuf, keystr, valstr);

            if (keystr == "Video")
            {
                is_video = true; is_audio = false;
                continue;
            }
            if (keystr == "Audio")
            {
                is_video = false; is_audio = true;
                continue;
            }
            if (keystr == "Text")
            {
                is_video = false; is_audio = false;
                continue;
            }

            if (keystr == "Duration" && is_video && video_duration == 0)
                video_duration = atoi(valstr.c_str());
        }
        file.close();
    }
    remove(tmpfile.c_str());

    return video_duration > 0;
}

// static
int FileCutThread::Execute(const char* cmd_line, bool isWait,
    bool isNeedConsole)
{
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    DWORD dwCreateflag = CREATE_NO_WINDOW;
    if (isNeedConsole)
    {
        dwCreateflag = CREATE_NEW_CONSOLE;
    }

    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.lpReserved = NULL;
    siStartInfo.lpReserved2 = NULL;
    siStartInfo.cbReserved2 = 0;
    siStartInfo.lpDesktop = NULL;
    siStartInfo.dwFlags = 0;
    siStartInfo.lpTitle = "";

    if (FALSE == CreateProcessA(NULL, (LPSTR)cmd_line, NULL, NULL, 0, dwCreateflag,
        NULL, NULL, &siStartInfo, &piProcInfo) ) {
            return -1;
    }

    if (false == isWait)
        return 0;

    if (WAIT_OBJECT_0 == WaitForSingleObject(piProcInfo.hProcess, INFINITE)) {
        DWORD exit_code;
        GetExitCodeProcess(piProcInfo.hProcess, &exit_code);
        return exit_code;
    }
    else {
        return -2;
    }
}
