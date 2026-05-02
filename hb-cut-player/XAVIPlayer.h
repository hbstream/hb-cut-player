#ifndef XAVIPLAYER_H
#define XAVIPLAYER_H

#include <QtWidgets/QMainWindow>
#include "ui_XAVIPlayer.h"
#include <QTimer>
#include <QFileInfo>

#include <map>
#include <string>
#include <vector>

#include "file_cut_thread.h"

class QProcess;
class VLCPlayer;
class VLCInstance;

class XAVIPlayer : public QMainWindow, public FileCutThread::Observer
{
    Q_OBJECT

public:
    explicit XAVIPlayer(QWidget *parent = nullptr);
    ~XAVIPlayer() override;

    void closeEvent(QCloseEvent* ev) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void OnFileCutComplete(int cutId) override;

signals:
    void SigPlay(QString playName, int seekTime);
    void SigFileCutComplete(int jobId);

private slots:
    void onUpdateTimer();

    void onBtnOpenClick();
    void onBtnStartClick();
    void onBtnStopClick();
    void onAudioSliderMove(int);
    void onVideoSliderMove(int);
    void onVideoSliderPress();
    void onVideoSliderRelease();
    void onBtnIncreaseClick();
    void onBtnDecreaseClick();
    void onBtnInc10Click();
    void onBtnDec10Click();
    void onBtnRateReset();

    void onClickBtnSetCutStart();
    void onClickBtnSetCutStop();
    void onClickBtnAddCutTask();
    void onClickBtnCut();
    void onClickBtnMerge();

    void slotPlay(QString playName, int seekTime);
    void slotDClickVideo();

    void slotFileCutComplete(int jobId);
    void slotMergeFinished(int exitCode);

private:
    void SeekPlay(int seekTime);
    void ReadConfig();
    int  CvtControKey(const std::string& keystr);

    void IncreasePlayRate(int rateVal);
    void DecreasePlayRate(int rateVal);

    FileCutThread* GuiCreateCutJob(bool toTable);

private:
    Ui::XAVIPlayerClass ui;

    QTimer*            timer_              = nullptr;
    unsigned long long check_state_timer_counts_ = 0;

    VLCPlayer*    player_       = nullptr;
    VLCInstance*  vlc_instance_ = nullptr;

    std::vector<QString> play_list_;
    int                  play_index_ = 0;
    QFileInfo            playing_fileinfo_;

    bool is_video_slider_moving_ = false;
    int  video_slider_move_pos_  = 0;
    bool is_playing_             = false;

    int  play_rate_              = 100;
    bool is_pressing_y_          = false;
    int  pressing_y_secs_        = 0;
    int  notpressing_key_secs_   = 0;
    int  delay_seconds_          = 0;
    int  control_key_            = 0;
    bool is_playing_key_         = false;
    int  keyfile_start_time_     = 0;
    int  seek_time_              = 0;

    bool is_first_play_          = true;

    std::map<int, FileCutThread*> cut_threads_;
    int                           cut_id_ = 0;

    // 自动命名计数器：每打开一个新视频归零，每次"添加剪切任务"递增。
    int task_counter_for_source_ = 0;

    // 已经成功剪切完毕的输出文件，按完成顺序记录，供合并使用。
    std::vector<std::string> completed_cut_files_;

    // 合并子进程：持有所有权以便发出 finished 信号后清理。
    QProcess*  merge_process_      = nullptr;
    QString    merge_output_path_;
    QString    merge_concat_list_path_;
};

#endif // XAVIPLAYER_H
