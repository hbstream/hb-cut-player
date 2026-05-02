#include "XAVIPlayer.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include <QFileDialog>
#include <QDir>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QSettings>
#include <QTime>
#include <QHeaderView>
#include <QBrush>
#include <QUrl>
#include <QCursor>
#include <QApplication>

#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")

#include "VLCInstance.h"
#include "VLCPlayer.h"
#include "base/str_helper.h"

namespace {

enum TableIndex
{
    kTableIndexId = 0,
    kTableIndexFile,
    kTableIndexTimeBeg,
    kTableIndexTimeEnd,
    kTableIndexOutName,
};

QString FormatHms(int totalSeconds)
{
    if (totalSeconds < 0) totalSeconds = 0;
    const int h = totalSeconds / 3600;
    const int m = (totalSeconds % 3600) / 60;
    const int s = totalSeconds % 60;
    return QString::asprintf("%02d:%02d:%02d", h, m, s);
}

} // namespace

XAVIPlayer::XAVIPlayer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(onUpdateTimer()));
    timer_->setInterval(250);

    check_state_timer_counts_ = 0;

    ui.sliderAudio->setMaximum(100);
    ui.sliderAudio->setValue(100);

    installEventFilter(this);
    ui.sliderAudio->installEventFilter(this);
    ui.sliderVideo->installEventFilter(this);
    ui.frameVideo->installEventFilter(this);

    is_video_slider_moving_ = false;
    video_slider_move_pos_ = 0;
    is_playing_ = false;

    is_pressing_y_ = false;
    pressing_y_secs_ = 0;
    is_playing_key_ = false;
    notpressing_key_secs_ = 0;
    delay_seconds_ = 0;
    keyfile_start_time_ = 0;
    seek_time_ = 0;
    control_key_ = 0x59;
    play_rate_ = 100;

    is_first_play_ = true;

    // 优先使用系统已安装的 VLC 插件目录；找不到再回退到工程内自带 vlc/plugins。
    QString plugin_dir = QStringLiteral("C:/Program Files/VideoLAN/VLC/plugins");
    if (!QDir(plugin_dir).exists()) {
        plugin_dir = QDir::currentPath() + QStringLiteral("/plugins");
        if (!QDir(plugin_dir).exists()) {
            plugin_dir = QDir::currentPath() + QStringLiteral("/vlc/plugins");
        }
    }
    std::string plugin_path = QDir::toNativeSeparators(plugin_dir).toUtf8().constData();

    vlc_instance_ = new VLCInstance(plugin_path);
    player_ = new VLCPlayer(vlc_instance_);
    player_->SetRenderWindow(reinterpret_cast<void*>(ui.frameVideo->winId()));

    connect(this, SIGNAL(SigPlay(QString, int)), this, SLOT(slotPlay(QString, int)), Qt::QueuedConnection);

    connect(ui.btnSetCutStart, SIGNAL(clicked()), this, SLOT(onClickBtnSetCutStart()));
    connect(ui.btnSetCutStop, SIGNAL(clicked()), this, SLOT(onClickBtnSetCutStop()));
    connect(ui.btnAddCutTask, SIGNAL(clicked()), this, SLOT(onClickBtnAddCutTask()));
    connect(ui.btnCut, SIGNAL(clicked()), this, SLOT(onClickBtnCut()));

    connect(this, SIGNAL(SigFileCutComplete(int)), this, SLOT(slotFileCutComplete(int)));

    play_index_ = 0;
    if (!play_list_.empty()) {
        emit SigPlay(play_list_[play_index_], 0);
    }

    cut_id_ = 0;

    setAcceptDrops(true);

    ui.tableCutTask->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableCutTask->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableCutTask->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tableCutTask->horizontalHeader()->setStretchLastSection(true);
    ui.tableCutTask->setColumnWidth(kTableIndexId, 60);
    ui.tableCutTask->setColumnWidth(kTableIndexFile, 200);
    ui.tableCutTask->setColumnWidth(kTableIndexTimeBeg, 100);
    ui.tableCutTask->setColumnWidth(kTableIndexTimeEnd, 100);
    ui.tableCutTask->verticalHeader()->setVisible(false);
}

XAVIPlayer::~XAVIPlayer()
{
    delete player_;
    delete vlc_instance_;
}

int XAVIPlayer::CvtControKey(const std::string& keystr)
{
    if (keystr.empty()) return 0;
    char c = keystr[0];
    if (c >= 'a' && c <= 'z') return 0x41 + (c - 'a');
    if (c >= 'A' && c <= 'Z') return 0x41 + (c - 'A');
    return 0;
}

void XAVIPlayer::ReadConfig()
{
    delay_seconds_ = 12;     // 3 秒，250ms 计时器一秒 4 拍
    control_key_ = 0x59;     // 默认按键 Y
}

void XAVIPlayer::slotPlay(QString playName, int seekTime)
{
    this->setFocus();

    play_rate_ = 100;
    ui.labelRate->setText(QStringLiteral("100%"));

    QString native_path = QDir::toNativeSeparators(playName);
    playing_fileinfo_ = QFileInfo(native_path);

    ui.editCutOutName->setText(playing_fileinfo_.completeBaseName() + "_cut." + playing_fileinfo_.suffix());

    player_->PlayFile(native_path.toUtf8().constData());
    player_->SetVolume(ui.sliderAudio->value());
    if (seekTime > 0) player_->SetPlayTime(seekTime);

    int duration = static_cast<int>(player_->GetDuration());
    ui.sliderVideo->setValue(0);
    ui.sliderVideo->setMaximum(duration);

    ui.btnStart->setText(QStringLiteral("⏸"));
    setWindowTitle(QStringLiteral("HB Cut Player - ") + playing_fileinfo_.fileName());

    timer_->start();
    is_playing_ = true;
}

void XAVIPlayer::SeekPlay(int seekTime)
{
    player_->SetPlayTime(seekTime);
    ui.sliderVideo->setValue(static_cast<int>(player_->GetPlayTime()));
    is_playing_ = true;
}

void XAVIPlayer::onBtnOpenClick()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
        this,
        QStringLiteral("打开视频"),
        QString(),
        tr("视频文件 (*.mp4 *.mkv *.avi *.mov *.flv *.wmv *.ts *.m4v);;所有文件 (*.*)"));
    if (filenames.isEmpty()) return;

    play_list_.clear();
    for (const auto& f : filenames) play_list_.push_back(f);
    play_index_ = 0;
    emit SigPlay(play_list_[play_index_], 0);
}

void XAVIPlayer::onBtnStartClick()
{
    // 用 ▶ / ⏸ 字符表示当前按钮状态。
    if (ui.btnStart->text() == QStringLiteral("▶"))
    {
        ui.btnStart->setText(QStringLiteral("⏸"));
        timer_->start();
        if (is_playing_) player_->Pause();
    }
    else
    {
        ui.btnStart->setText(QStringLiteral("▶"));
        timer_->stop();
        player_->Pause();
    }
}

void XAVIPlayer::onBtnStopClick()
{
    player_->Stop();
    timer_->stop();
    is_playing_ = false;
    ui.btnStart->setText(QStringLiteral("▶"));
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

    int duration = static_cast<int>(player_->GetDuration());
    ui.labelTime->setText(FormatHms(val) + " / " + FormatHms(duration));
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
    SeekPlay(video_slider_move_pos_);
}

void XAVIPlayer::onBtnIncreaseClick()  { IncreasePlayRate(5); }
void XAVIPlayer::onBtnDecreaseClick()  { DecreasePlayRate(5); }
void XAVIPlayer::onBtnInc10Click()     { IncreasePlayRate(10); }
void XAVIPlayer::onBtnDec10Click()     { DecreasePlayRate(10); }

void XAVIPlayer::IncreasePlayRate(int rateVal)
{
    if (!is_playing_) return;
    play_rate_ += rateVal;
    player_->SetPlayRate(play_rate_ / 100.0f);
    ui.labelRate->setText(QString::asprintf("%d%%", play_rate_));
}

void XAVIPlayer::DecreasePlayRate(int rateVal)
{
    if (!is_playing_) return;
    play_rate_ -= rateVal;
    if (play_rate_ <= 0) play_rate_ = 5;
    player_->SetPlayRate(play_rate_ / 100.0f);
    ui.labelRate->setText(QString::asprintf("%d%%", play_rate_));
}

void XAVIPlayer::onBtnRateReset()
{
    if (!is_playing_) return;
    play_rate_ = 100;
    player_->SetPlayRate(1.0f);
    ui.labelRate->setText(QStringLiteral("100%"));
}

void XAVIPlayer::onClickBtnSetCutStart()
{
    if (!player_) return;
    int now_secs = static_cast<int>(player_->GetPlayTime());
    ui.editCutTimeBeg->setText(FormatHms(now_secs));
}

void XAVIPlayer::onClickBtnSetCutStop()
{
    if (!player_) return;
    int now_secs = static_cast<int>(player_->GetPlayTime());
    ui.editCutTimeEnd->setText(FormatHms(now_secs));
}

void XAVIPlayer::onClickBtnAddCutTask()
{
    GuiCreateCutJob(true);
}

FileCutThread* XAVIPlayer::GuiCreateCutJob(bool toTable)
{
    if (!playing_fileinfo_.exists()) {
        QMessageBox::warning(this, QStringLiteral("提示"),
            QStringLiteral("请先打开一个视频文件"), QMessageBox::Ok);
        return nullptr;
    }

    QTime cut_beg = QTime::fromString(ui.editCutTimeBeg->text(), "HH:mm:ss");
    QTime cut_end = QTime::fromString(ui.editCutTimeEnd->text(), "HH:mm:ss");

    if (!cut_beg.isValid() || !cut_end.isValid()) {
        QMessageBox::warning(this, QStringLiteral("参数错误"),
            QStringLiteral("时间格式应为 HH:mm:ss"), QMessageBox::Ok);
        return nullptr;
    }

    int cut_millsecs = cut_end.msecsSinceStartOfDay() - cut_beg.msecsSinceStartOfDay();
    if (cut_millsecs <= 0) {
        QMessageBox::warning(this, QStringLiteral("参数错误"),
            QStringLiteral("结束时间必须晚于开始时间"), QMessageBox::Ok);
        return nullptr;
    }

    std::string from_name = strhelper::Wide2Ansi(strhelper::Utf82Wide(
        playing_fileinfo_.canonicalFilePath().toUtf8().constData()));
    std::string to_name = strhelper::Wide2Ansi(strhelper::Utf82Wide(
        (playing_fileinfo_.canonicalPath() + "/" + ui.editCutOutName->text()).toUtf8().constData()));

    FileCutThread* cut_job = new FileCutThread(cut_id_, this,
        from_name, to_name, cut_beg.msecsSinceStartOfDay() / 1000, 0, cut_millsecs / 1000,
        false, std::tm{});
    cut_threads_[cut_id_] = cut_job;

    if (toTable) {
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
    if (ui.tableCutTask->rowCount() == 0) {
        if (!GuiCreateCutJob(true)) return;
    }

    if (!cut_threads_.empty()) {
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
    for (int i = 0; i < rowcount; ++i) {
        if (ui.tableCutTask->item(i, kTableIndexId)->text().toInt() == jobId) {
            // Qt6 用 setBackground(QBrush) 替换 Qt5 的 setBackgroundColor。
            ui.tableCutTask->item(i, kTableIndexOutName)->setBackground(QBrush(QColor(0x2e, 0x8b, 0x57)));
            break;
        }
    }

    auto it = cut_threads_.find(jobId);
    if (it != cut_threads_.end()) {
        it->second->Stop();
        delete it->second;
        cut_threads_.erase(it);
    }

    if (cut_threads_.empty()) {
        ui.btnCut->setEnabled(true);
    } else {
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

    if (check_state_timer_counts_ % 2 == 0) {
        if (!is_playing_ || is_video_slider_moving_) return;

        if (is_first_play_) {
            if (!player_->IsPlaying()) return;
            is_first_play_ = false;
        }

        if (!player_->IsPlaying()) {
            // 当前文件播放完毕，自动切到下一个。
            if (!play_list_.empty()) {
                play_index_ = (play_index_ + 1) % static_cast<int>(play_list_.size());
                emit SigPlay(play_list_[play_index_], 0);
            }
            return;
        }

        const int now_secs = static_cast<int>(player_->GetPlayTime());
        const int total_secs = static_cast<int>(player_->GetDuration());
        ui.sliderVideo->setValue(now_secs);
        ui.labelTime->setText(FormatHms(now_secs) + " / " + FormatHms(total_secs));
    }
}

void XAVIPlayer::closeEvent(QCloseEvent* /*ev*/)
{
}

bool XAVIPlayer::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEv = static_cast<QMouseEvent*>(ev);
        // Qt6: QMouseEvent::x() 弃用，改用 position().toPoint().x()。
        const int x = static_cast<int>(mouseEv->position().x());
        if (obj == ui.sliderVideo && ui.sliderVideo->maximum()) {
            int slider_pos = ui.sliderVideo->value() * ui.sliderVideo->width() / ui.sliderVideo->maximum();
            if (std::abs(slider_pos - x) > 10) {
                int val = x * ui.sliderVideo->maximum() / ui.sliderVideo->width();
                ui.sliderVideo->setValue(val);
                if (is_playing_) SeekPlay(val);
                return true;
            }
            return QWidget::eventFilter(obj, ev);
        }
        if (obj == ui.sliderAudio) {
            int slider_pos = ui.sliderAudio->value() * ui.sliderAudio->width() / ui.sliderAudio->maximum();
            if (std::abs(slider_pos - x) > 10) {
                int val = x * ui.sliderAudio->maximum() / ui.sliderAudio->width();
                ui.sliderAudio->setValue(val);
                if (is_playing_) player_->SetVolume(ui.sliderAudio->value());
                return true;
            }
            return QWidget::eventFilter(obj, ev);
        }
        return QWidget::eventFilter(obj, ev);
    }

    if (ev->type() == QEvent::KeyPress) {
        QKeyEvent *k = static_cast<QKeyEvent *>(ev);
        const bool is_use_ctrl = k->modifiers() & Qt::ControlModifier;
        const bool is_use_shift = k->modifiers() & Qt::ShiftModifier;

        if (player_) {
            if (k->key() == Qt::Key_Left) {
                int delta = is_use_shift ? -60 : (is_use_ctrl ? -30 : -5);
                SeekPlay(static_cast<int>(player_->GetPlayTime()) + delta);
                return true;
            }
            if (k->key() == Qt::Key_Right) {
                int delta = is_use_shift ? 60 : (is_use_ctrl ? 30 : 5);
                SeekPlay(static_cast<int>(player_->GetPlayTime()) + delta);
                return true;
            }
            if (k->key() == Qt::Key_Space) {
                onBtnStartClick();
                return true;
            }
            if (k->key() == Qt::Key_Escape && isFullScreen()) {
                showNormal();
                return true;
            }
        }
        return QWidget::eventFilter(obj, ev);
    }

    return QWidget::eventFilter(obj, ev);
}

void XAVIPlayer::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void XAVIPlayer::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    for (auto it = urls.begin(); it != urls.end(); ++it) {
        emit SigPlay(it->toLocalFile(), 0);
        break;
    }
}

void XAVIPlayer::mouseDoubleClickEvent(QMouseEvent* /*event*/)
{
    QWidget* w = QApplication::widgetAt(QCursor::pos());
    if (w == ui.frameVideo) {
        if (isFullScreen()) showNormal();
        else                showFullScreen();
    }
}
