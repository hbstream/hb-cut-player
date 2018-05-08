#ifndef XAVIPLAYER_H
#define XAVIPLAYER_H

#include <QtWidgets/QMainWindow>
#include "ui_XAVIPlayer.h"
#include <QTimer>
#include <QFileInfo>
#include <string>
#include <vector>

#include "file_cut_thread.h"

#include <map>

struct KeyFrame 
{
    double time;
    double pos;
};

class VLCPlayer;
class VLCInstance;


class QLabel;

class XAVIPlayer : public QMainWindow, public FileCutThread::Observer
{
    Q_OBJECT

public:
    XAVIPlayer(QWidget *parent = 0);
    ~XAVIPlayer();

    virtual void closeEvent(QCloseEvent* ev);

    virtual bool eventFilter(QObject *, QEvent *);

    virtual void dragEnterEvent(QDragEnterEvent *event);

    virtual void dropEvent(QDropEvent *event);

    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void OnFileCutComplete(int cutId);

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

    void slotPlay(QString playName, int seekTime);
    void slotDClickVideo();

    void slotFileCutComplete(int jobId);

private:
    void SeekPlay(int seekTime);
    void ReadConfig();
    int CvtControKey(const std::string& keystr);

    void IncreasePlayRate(int rateVal);
    void DecreasePlayRate(int rateVal);

    void IncreasePlayTime(int timeVal);
    void DecreasePlayTime(int timeVal);

    FileCutThread* GuiCreateCutJob(bool toTable);

private:
    Ui::XAVIPlayerClass ui;

    QTimer* timer_;
    unsigned __int64 check_state_timer_counts_;

    VLCPlayer* player_;
    VLCInstance* vlc_instance_;
    //VLCListPlayer* list_player_;

    std::vector<QString> play_list_;
    //QString key_file_;
    int play_index_;
    QFileInfo playing_fileinfo_;

    bool is_video_slider_moving_;
    int video_slider_move_pos_;
    bool is_playing_;

    QLabel* label_;
    int play_rate_;
    bool is_pressing_y_;
    int pressing_y_secs_;
    int notpressing_key_secs_;
    int delay_seconds_;
    int control_key_;
    bool is_playing_key_;
    int keyfile_start_time_;
    int seek_time_;

    bool is_first_play_;

    std::map<int, FileCutThread*> cut_threads_;
    int cut_id_;
};

#endif // XAVIPLAYER_H
