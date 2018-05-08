#include "XAVIPlayer.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>

#include <QFileDialog>
#include <QDir>
#include <qevent.h>
#include <QLabel>
#include <QSettings>
#include <QTime>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>

#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")

#include "VLCInstance.h"
#include "VLCPlayer.h"
#include "Bitstream.h"
#include "base/str_helper.h"

void LogInfo(const char* msg, const char* func, int linno)
{
//     FILE* fp = fopen("xaviplayer.log", "a+");
//     fprintf(fp, "<%s:%d> %s\n", func, linno, msg);
//     fclose(fp);
}

#define LOGINFO(msg) LogInfo(msg, __FUNCTION__, __LINE__)

enum TableIndex
{
    kTableIndexId = 0,
    kTableIndexFile,
    kTableIndexTimeBeg,
    kTableIndexTimeEnd,
    kTableIndexOutName,
};

XAVIPlayer::XAVIPlayer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

#ifndef _DEBUG
    //QDir::setCurrent(QCoreApplication::applicationDirPath());
#endif

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(onUpdateTimer()));
    timer_->setInterval(250);

    check_state_timer_counts_ = 0;

    ui.sliderAudio->setMaximum(100);
    ui.sliderAudio->setValue(100);

    installEventFilter(this);
    ui.sliderAudio->installEventFilter(this);
    ui.sliderVideo->installEventFilter(this);
    ui.frame->installEventFilter(this);

    label_ = new QLabel(QStringLiteral("演示程序-haibindev"), this);
    label_->setStyleSheet("background-color: rgb(236, 233, 216);");
    label_->setMinimumWidth(50);
    label_->setMaximumWidth(50);
    label_->setMinimumHeight(20);
    label_->setMaximumHeight(20);
    label_->move(10, 10);

    is_video_slider_moving_ = false;
    video_slider_move_pos_ = 0;
    is_playing_ = false;
    //last_open_file_ = "";

    is_pressing_y_ = false;
    pressing_y_secs_ = 0;
    is_playing_key_ = false;
    notpressing_key_secs_ = 0;
    delay_seconds_ = 0;
    keyfile_start_time_ = 0;

    is_first_play_ = true;

    std::string plugin_path = QDir::currentPath().replace("/", "\\").toUtf8();
    plugin_path += "\\vlc\\plugins\\";
    vlc_instance_ = new VLCInstance(plugin_path);
    player_ = new VLCPlayer(vlc_instance_);
    player_->SetRenderWindow((void*)ui.frame->winId());
    //player_->SetFullScreen(true);

//     list_player_ = new VLCListPlayer(vlc_instance_);
//     list_player_->SetRenderWindow(ui.frame->winId());

    connect(this, SIGNAL(SigPlay(QString, int)), this, SLOT(slotPlay(QString, int)), Qt::QueuedConnection);

    connect(ui.btnSetCutStart, SIGNAL(clicked()), this, SLOT(onClickBtnSetCutStart()));
    connect(ui.btnSetCutStop, SIGNAL(clicked()), this, SLOT(onClickBtnSetCutStop()));
    connect(ui.btnAddCutTask, SIGNAL(clicked()), this, SLOT(onClickBtnAddCutTask()));
    connect(ui.btnCut, SIGNAL(clicked()), this, SLOT(onClickBtnCut()));

    connect(this, SIGNAL(SigFileCutComplete(int)), this, SLOT(slotFileCutComplete(int)));

    play_index_ = 0;
    //ReadConfig();
    if (play_list_.size())
    {
        //list_player_->PlayList(play_list_);
        emit SigPlay(play_list_[play_index_], 0);
    }

    //cut_thread_ = NULL;
    cut_id_ = 0;

    setAcceptDrops(true);

    ui.tableCutTask->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableCutTask->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableCutTask->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tableCutTask->horizontalHeader()->setStretchLastSection(true);
    ui.tableCutTask->setColumnWidth(kTableIndexId, 50);
    ui.tableCutTask->setColumnWidth(kTableIndexFile, 100);
    ui.tableCutTask->setColumnWidth(kTableIndexTimeBeg, 100);
    ui.tableCutTask->setColumnWidth(kTableIndexTimeEnd, 100);
    ui.tableCutTask->setColumnWidth(kTableIndexOutName, 100);
}

XAVIPlayer::~XAVIPlayer()
{
    delete player_;
    delete vlc_instance_;
}

void StrTrim(std::string& str)
{
    std::string::iterator it = std::find_if(str.begin(),str.end(), std::not1(std::ptr_fun(isspace)));
    str.erase(str.begin(), it);
    std::string::reverse_iterator rit = std::find_if(str.rbegin(),str.rend(),std::not1(std::ptr_fun(isspace))); 
    str.erase(rit.base(), str.end());
}

int XAVIPlayer::CvtControKey(const std::string& keystr)
{
    if (keystr == "A" || keystr == "a") return 0x41;
    else if (keystr == "B" || keystr == "b") return 0x42;
    else if (keystr == "C" || keystr == "c") return 0x43;
    else if (keystr == "D" || keystr == "d") return 0x44;
    else if (keystr == "E" || keystr == "e") return 0x45;
    else if (keystr == "F" || keystr == "f") return 0x46;
    else if (keystr == "G" || keystr == "g") return 0x47;
    else if (keystr == "H" || keystr == "h") return 0x48;
    else if (keystr == "I" || keystr == "i") return 0x49;
    else if (keystr == "J" || keystr == "j") return 0x4a;
    else if (keystr == "K" || keystr == "k") return 0x4b;
    else if (keystr == "L" || keystr == "l") return 0x4c;
    else if (keystr == "M" || keystr == "m") return 0x4d;
    else if (keystr == "N" || keystr == "n") return 0x4e;
    else if (keystr == "O" || keystr == "o") return 0x4f;
    else if (keystr == "P" || keystr == "p") return 0x50;
    else if (keystr == "Q" || keystr == "q") return 0x51;
    else if (keystr == "R" || keystr == "r") return 0x52;
    else if (keystr == "S" || keystr == "s") return 0x53;
    else if (keystr == "T" || keystr == "t") return 0x54;
    else if (keystr == "U" || keystr == "u") return 0x55;
    else if (keystr == "V" || keystr == "v") return 0x56;
    else if (keystr == "W" || keystr == "w") return 0x57;
    else if (keystr == "X" || keystr == "x") return 0x58;
    else if (keystr == "Y" || keystr == "y") return 0x59;
    else if (keystr == "Z" || keystr == "z") return 0x5a;
    else return 0x00;
}

void XAVIPlayer::ReadConfig()
{
    delay_seconds_ = 3;
    delay_seconds_ *= 4;
    control_key_ = 0x59;

    std::ifstream input("c:\\list.txt");
    if (input.is_open())
    {
        while (true)
        {
            if (input.eof()) break;
            std::string line;
            std::getline(input, line);
            StrTrim(line);
            if (line.empty()) continue;

            int pos = line.find("=");
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string val = line.substr(pos+1);
                StrTrim(key);
                StrTrim(val);
                if (key == "file")
                {
                    play_list_.push_back(QString::fromStdString(val));
                }
//                 else if (key == "keyfile")
//                 {
//                     key_file_ = QString::fromStdString(val);
//                 }
                else if (key == "delay_seconds")
                {
                    delay_seconds_ = atoi(val.c_str());
                    delay_seconds_ *= 4;
                }
                else if (key == "control_key")
                {
                    control_key_ = CvtControKey(val);
                }
//                 else if (key == "keyfile_start_time")
//                 {
//                     keyfile_start_time_ = atoi(val.c_str());
//                 }
                else if (key == "seek_time")
                {
                    seek_time_ = atoi(val.c_str());
                }
            }
        }
        input.close();
    }
}

void XAVIPlayer::slotPlay(QString playName, int seekTime)
{
    LOGINFO("fetch basic info");

    this->setFocus();

    play_rate_ = 100;

    QString play_path = playName.replace("/", "\\").toUtf8();

    playing_fileinfo_ = QFileInfo(play_path);

    ui.editCutOutName->setText(playing_fileinfo_.completeBaseName() + "_." + playing_fileinfo_.suffix());

    player_->PlayFile(playName.replace("/", "\\").toUtf8().data());
    //player_->SetVolume(ui.sliderAudio->value());
    player_->SetVolume(100);
    if (seekTime > 0) player_->SetPlayTime(seekTime);

    double duration = player_->GetDuration();
    ui.sliderVideo->setValue(0);
    ui.sliderVideo->setMaximum(duration);
     
    ui.btnStart->setText(QStringLiteral("暂停"));

    timer_->start();
    is_playing_ = true;
}

void XAVIPlayer::SeekPlay(int seekTime)
{
    player_->SetPlayTime(seekTime);
    ui.sliderVideo->setValue(player_->GetPlayTime());
    is_playing_ = true;
}

void XAVIPlayer::onBtnOpenClick()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
        this,
        QStringLiteral("打开视频"),
        QString(),
        tr("All files(*.*)")
        );
    if (!filenames.isEmpty())
    {
         play_list_.clear();
         for (QStringList::iterator it = filenames.begin();
             it != filenames.end(); ++it)
         {
             play_list_.push_back(*it);
         }
         play_index_ = 0;
         emit SigPlay(play_list_[play_index_], 0);
    }
}

void XAVIPlayer::onBtnStartClick()
{
    if (ui.btnStart->text() == QStringLiteral("继续"))
    {
        ui.btnStart->setText(QStringLiteral("暂停"));
        timer_->start();
        if (is_playing_) player_->Pause();
    }
    else
    {
        ui.btnStart->setText(QStringLiteral("继续"));
        timer_->stop();
        player_->Pause();
    }
}

void XAVIPlayer::onBtnStopClick()
{
    player_->Stop();
    timer_->stop();
    is_playing_ = false;
}

void XAVIPlayer::onAudioSliderMove(int v)
{
    if (!is_playing_) return;
    player_->SetVolume(v);
}

void XAVIPlayer::onVideoSliderMove(int val)
{
    if (!is_playing_) return;
    video_slider_move_pos_ = val;

    QTime duration(0, 0, player_->GetDuration());
    QTime now(0, 0, video_slider_move_pos_);

    QString durastr = duration.toString("hh:mm:ss");
    QString nowstr = now.toString("hh:mm:ss");
    QString infostr = nowstr + "/" + durastr;
    ui.labelTime->setText(infostr);
}

void XAVIPlayer::onVideoSliderPress()
{
    if (!is_playing_) return;
    is_video_slider_moving_ = true;
}

void XAVIPlayer::onVideoSliderRelease()
{
    if (!is_playing_) return;
    is_video_slider_moving_ = false;

    LOGINFO("play url");

    SeekPlay(video_slider_move_pos_);
}

void XAVIPlayer::onBtnIncreaseClick()
{
    IncreasePlayRate(5);
}

void XAVIPlayer::onBtnDecreaseClick()
{
    DecreasePlayRate(5);
}

void XAVIPlayer::onBtnInc10Click()
{
    IncreasePlayRate(10);
}

void XAVIPlayer::onBtnDec10Click()
{
    DecreasePlayRate(10);
}

void XAVIPlayer::IncreasePlayRate(int rateVal)
{
    if (is_playing_)
    {
        play_rate_ += rateVal;
        player_->SetPlayRate(play_rate_ / 100.0);
        QString txtstr;
        txtstr.sprintf("%d%%", play_rate_);
        label_->setText(txtstr);
    }
}

void XAVIPlayer::DecreasePlayRate(int rateVal)
{
    if (is_playing_)
    {
        play_rate_ -= rateVal;
        if (play_rate_ <= 0)
        {
            play_rate_ = 5;
        }
        player_->SetPlayRate(play_rate_ / 100.0);
        QString txtstr;
        txtstr.sprintf("%d%%", play_rate_);
        label_->setText(txtstr);
    }
}

void XAVIPlayer::onBtnRateReset()
{
    if (is_playing_)
    {
        play_rate_ = 100;
        player_->SetPlayRate(play_rate_ / 100.0);
        QString txtstr;
        txtstr.sprintf("%d%%", play_rate_);
        label_->setText(txtstr);
    }
}

void XAVIPlayer::onClickBtnSetCutStart()
{
    if (player_)
    {
        int now_secs = player_->GetPlayTime();
        int now_minute = now_secs % 3600;
        QTime now(now_secs / 3600, now_minute / 60, now_minute % 60);
        QString nowstr = now.toString("hh:mm:ss");

        ui.editCutTimeBeg->setText(nowstr);
    }
}

void XAVIPlayer::onClickBtnSetCutStop()
{
    if (player_)
    {
        int now_secs = player_->GetPlayTime();
        int now_minute = now_secs % 3600;
        QTime now(now_secs / 3600, now_minute / 60, now_minute % 60);
        QString nowstr = now.toString("hh:mm:ss");

        ui.editCutTimeEnd->setText(nowstr);
    }
}

void XAVIPlayer::onClickBtnAddCutTask()
{
    GuiCreateCutJob(true);
}

FileCutThread* XAVIPlayer::GuiCreateCutJob(bool toTable)
{
    QTime cut_beg = QTime::fromString(ui.editCutTimeBeg->text());
    QTime cut_end = QTime::fromString(ui.editCutTimeEnd->text());

    int cut_millsecs = cut_end.msecsSinceStartOfDay() - cut_beg.msecsSinceStartOfDay();
    if (cut_millsecs <= 0)
    {
        QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("参数错误"),
            QMessageBox::Ok);
        return NULL;
    }

    std::string frome_name = strhelper::Wide2Ansi(strhelper::Utf82Wide(playing_fileinfo_.canonicalFilePath().toUtf8().data()));
    std::string to_name = strhelper::Wide2Ansi(strhelper::Utf82Wide((playing_fileinfo_.canonicalPath() + "/" + ui.editCutOutName->text()).toUtf8().data()));
    FileCutThread* cut_job = new FileCutThread(cut_id_, this,
        frome_name, to_name, cut_beg.msecsSinceStartOfDay() / 1000, 0, cut_millsecs / 1000,
        false, tm());
    cut_threads_[cut_id_] = cut_job;

    if (toTable)
    {
        int rowcount = ui.tableCutTask->rowCount();
        ui.tableCutTask->setRowCount(rowcount + 1);
        ui.tableCutTask->setItem(rowcount, kTableIndexId, new QTableWidgetItem(QString::number(cut_id_)));
        ui.tableCutTask->setItem(rowcount, kTableIndexFile, new QTableWidgetItem(playing_fileinfo_.fileName()));
        ui.tableCutTask->setItem(rowcount, kTableIndexTimeBeg, new QTableWidgetItem(ui.editCutTimeBeg->text()));
        ui.tableCutTask->setItem(rowcount, kTableIndexTimeEnd, new QTableWidgetItem(ui.editCutTimeEnd->text()));
        ui.tableCutTask->setItem(rowcount, kTableIndexOutName, new QTableWidgetItem(ui.editCutOutName->text()));
    }

    cut_id_++;

    return cut_job;
}

void XAVIPlayer::onClickBtnCut()
{
    if (ui.tableCutTask->rowCount() == 0)
    {
        GuiCreateCutJob(true);
    }

    if (cut_threads_.size() > 0)
    {
        FileCutThread* cut_job = cut_threads_.begin()->second;
        cut_job->Start();

        ui.btnCut->setEnabled(false);
    }
}

void XAVIPlayer::OnFileCutComplete(int cutId)
{
    emit SigFileCutComplete(cutId);
}

void XAVIPlayer::slotFileCutComplete(int jobId)
{
    int rowcount = ui.tableCutTask->rowCount();
    for (int i = 0; i < rowcount; ++i)
    {
        if (ui.tableCutTask->item(i, kTableIndexId)->text().toInt() == jobId)
        {
            ui.tableCutTask->item(i, kTableIndexOutName)->setBackgroundColor(Qt::green);
            break;
        }
    }

    auto it = cut_threads_.find(jobId);
    if (it != cut_threads_.end())
    {
        it->second->Stop();
        delete it->second;
        cut_threads_.erase(it);
    }

    if (cut_threads_.empty())
    {
        ui.btnCut->setEnabled(true);
    }
    else
    {
        FileCutThread* cut_job = cut_threads_.begin()->second;
        cut_job->Start();
    }
}

void XAVIPlayer::slotDClickVideo()
{
    showFullScreen();
}

void XAVIPlayer::onUpdateTimer()
{
    check_state_timer_counts_++;

    if (GetAsyncKeyState(control_key_) & 0x8000)
        is_pressing_y_ = true;
    else
        is_pressing_y_ = false;

    if (is_playing_key_)
    {
        if (is_pressing_y_)
        {
            notpressing_key_secs_ = 0;
            player_->SetVolume(64);
        }
        else
        {
            notpressing_key_secs_++;
            if (notpressing_key_secs_ >= delay_seconds_)
            {
                play_index_++;
                if (play_index_ == play_list_.size())
                {
                    play_index_ = 0;
                }
                emit SigPlay(play_list_[play_index_], 0);
                is_playing_key_ = false;
                return;
            }
            else
            {
                player_->SetVolume(64.0 * (delay_seconds_-notpressing_key_secs_-1)/delay_seconds_);
            }
        }
    }
    else
    {
        if (is_pressing_y_)
        {
            pressing_y_secs_++;
            if (pressing_y_secs_ >= delay_seconds_)
            {
                //emit SigPlay(key_file_, 0);
                //list_player_->PlayNext();
                emit SigPlay(play_list_[0], seek_time_);
                pressing_y_secs_ = 0;
                is_playing_key_ = true;
                return;
            }
            else
            {
                player_->SetVolume(64.0 * (delay_seconds_-pressing_y_secs_-1)/delay_seconds_);
            }
        }
        else
        {
            pressing_y_secs_ = 0;
            player_->SetVolume(64);
        }
    }

    if (check_state_timer_counts_ % 2 == 0)
    {
        this->setFocus();

        if (!is_playing_ || is_video_slider_moving_) return;

        if (is_first_play_)
        {
            if (false == player_->IsPlaying()) return;
            else is_first_play_ = false;
        }

        if (false == player_->IsPlaying())
        {
            if (is_playing_key_ || notpressing_key_secs_ < delay_seconds_)
            {
                //emit SigPlay(key_file_, keyfile_start_time_);
                emit SigPlay(play_list_[0], seek_time_);
                pressing_y_secs_ = 0;
                is_playing_key_ = true;
            }
            else
            {
                play_index_++;
                if (play_index_ == play_list_.size())
                {
                    play_index_ = 0;
                }
                emit SigPlay(play_list_[play_index_], 0);
                is_playing_key_ = false;
            }
        }
        else if (false == is_playing_key_ && player_->GetPlayTime() >= seek_time_-1 &&
            pressing_y_secs_ < delay_seconds_)
        {
            emit SigPlay(play_list_[0], 0);
        }
        else
        {
            ui.sliderVideo->setValue(player_->GetPlayTime());

            int secs = player_->GetDuration();
            int minute = secs % 3600;
            QTime duration(secs / 3600, minute / 60, minute % 60);
            int now_secs = player_->GetPlayTime();
            int now_minute = now_secs % 3600;
            QTime now(now_secs / 3600, now_minute / 60, now_minute % 60);

            QString durastr = duration.toString("hh:mm:ss");
            QString nowstr = now.toString("hh:mm:ss");
            QString infostr = nowstr + "/" + durastr;
            ui.labelTime->setText(infostr);
        }
    }
}

void XAVIPlayer::closeEvent(QCloseEvent* ev)
{

}

bool XAVIPlayer::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEv = static_cast<QMouseEvent*>(ev);
        int x = mouseEv->x();
        if (obj == ui.sliderVideo && ui.sliderVideo->maximum())
        {
            int slider_pos = ui.sliderVideo->value() * ui.sliderVideo->width() / ui.sliderVideo->maximum();
            if (abs(slider_pos - x) > 10)
            {
                int val = x  * ui.sliderVideo->maximum() / ui.sliderVideo->width();
                ui.sliderVideo->setValue(val);
                if (is_playing_) SeekPlay(val);
            }
            else
            {
                return QWidget::eventFilter(obj, ev);
            }
        }
        else if (obj == ui.sliderAudio)
        {
            int slider_pos = ui.sliderAudio->value() * ui.sliderAudio->width() / ui.sliderAudio->maximum();
            if (abs(slider_pos - x) > 10)
            {
                int val = x  * ui.sliderAudio->maximum() / ui.sliderAudio->width();
                ui.sliderAudio->setValue(val);
                if (is_playing_)  player_->SetVolume(ui.sliderAudio->value());
            }
            else
            {
                return QWidget::eventFilter(obj, ev);
            }
        }
        return true;
    }
    else if (ev->type() == QEvent::KeyPress)
    {
        QKeyEvent *k = (QKeyEvent *)ev;

        bool is_use_ctrl = k->modifiers() & Qt::ControlModifier;
        bool is_use_shift = k->modifiers() & Qt::ShiftModifier;

        if (player_)
        {
            if (k->key() == Qt::Key_Left)
            {
                if (is_use_shift)
                    SeekPlay(player_->GetPlayTime() - 60);
                else if (is_use_ctrl)
                    SeekPlay(player_->GetPlayTime() - 30);
                else
                    SeekPlay(player_->GetPlayTime() - 5);
            }
            else if (k->key() == Qt::Key_Right)
            {
                if (is_use_shift)
                    SeekPlay(player_->GetPlayTime() + 60);
                else if (is_use_ctrl)
                    SeekPlay(player_->GetPlayTime() + 30);
                else
                    SeekPlay(player_->GetPlayTime() + 5);
            }
        }
//         if (k->key() == Qt::Key_Y)
//         {
//             //             if (this->isFullScreen())
//             //             {
//             //                 this->showNormal();
//             //             }
//             //             else
//             //             {
//             //                 this->showFullScreen();
//             //             }
//             is_pressing_y_ = true;
//             a++;
//         }
//         keyPressEvent(k);
//     }
//     else if (ev->type() == QEvent::KeyRelease)
//     {
//         QKeyEvent *k = (QKeyEvent *)ev;
//         if (k->key() == Qt::Key_Y && false == k->isAutoRepeat())
//         {
//             is_pressing_y_ = false;
//             pressing_y_secs_ = 0;
//             b++;
//         }
        return true;
    }
    else
    {
        return QWidget::eventFilter(obj, ev);
    }
}

void XAVIPlayer::dragEnterEvent(QDragEnterEvent *event)
{
    //if (event->mimeData()->hasFormat())
    {
        event->acceptProposedAction();
    }
}

void XAVIPlayer::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    for (auto it = urls.begin(); it != urls.end(); ++it)
    {
        emit SigPlay(it->toLocalFile(), 0);

        break;
    }
}

void XAVIPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget* w = QApplication::widgetAt(QCursor::pos());
    if (w == ui.frame)
    {
        if (isFullScreen())
        {
            showNormal();
        }
        else
        {
            showFullScreen();
        }
    }
}

