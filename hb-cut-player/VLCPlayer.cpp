#include "VLCPlayer.h"

#include "vlc/vlc.h"

#include "VLCInstance.h"

VLCPlayer::VLCPlayer(VLCInstance* vlcInst)
    : vlc_inst_(vlcInst), vlc_media_(0), is_playing_(false)
{
    if (vlc_inst_)
    {
        vlc_player_ = libvlc_media_player_new(vlc_inst_->Get());
        libvlc_video_set_key_input(vlc_player_, false);
        libvlc_video_set_mouse_input(vlc_player_, false);
    }
}

VLCPlayer::~VLCPlayer()
{
    if (vlc_player_) libvlc_media_player_release(vlc_player_);
}

bool VLCPlayer::IsPlaying()
{
    if (libvlc_media_player_is_playing(vlc_player_) == 1)
    {
        return true;
    }
    return false;
}

void VLCPlayer::SetRenderWindow(void* wndHandle)
{
    if (vlc_player_ && wndHandle)
    {
        libvlc_media_player_set_hwnd(vlc_player_, wndHandle);
    }
}

void VLCPlayer::PlayUrl(const char* url)
{
    vlc_media_ = libvlc_media_new_location(vlc_inst_->Get(), url);
    if (vlc_media_)
    {
        libvlc_media_player_set_media(vlc_player_, vlc_media_);
        libvlc_media_release(vlc_media_);

        libvlc_media_player_play(vlc_player_);
        is_playing_ = true;
    }
}

void VLCPlayer::PlayFile(const char* filepath)
{
    vlc_media_ = libvlc_media_new_path(vlc_inst_->Get(), filepath);
    if (vlc_media_)
    {
        libvlc_media_parse(vlc_media_);
        duration_ = libvlc_media_get_duration(vlc_media_);

        libvlc_media_player_set_media(vlc_player_, vlc_media_);
        libvlc_media_release(vlc_media_);

        
        int err = libvlc_media_player_play(vlc_player_);
        is_playing_ = true;
    }
}

void VLCPlayer::Stop()
{
    if (vlc_player_) libvlc_media_player_stop(vlc_player_);
    {
        FILE* fp = fopen("test.txt", "a+");
        fprintf(fp, "1\n");
        fclose(fp);
    }
}

void VLCPlayer::Pause()
{
    if (vlc_player_) libvlc_media_player_pause(vlc_player_);
}

void VLCPlayer::SetVolume(int volume)
{
    libvlc_audio_set_volume(vlc_player_, volume);
}

int VLCPlayer::GetVolume()
{
    int volume = libvlc_audio_get_volume(vlc_player_);
    return volume;
}

void VLCPlayer::SetPlayRate(float rate)
{
    libvlc_media_player_set_rate(vlc_player_, rate);
}

float VLCPlayer::GetPlayRate()
{
    return libvlc_media_player_get_rate(vlc_player_);
}

double VLCPlayer::GetDuration()
{
    if (duration_ == 0)
    {
        duration_ = libvlc_media_get_duration(vlc_media_);
    }
    return double(duration_) / 1000;
}

void VLCPlayer::SetPlayTime(double seekTime)
{
    libvlc_media_player_set_time(vlc_player_, seekTime*1000);
}

double VLCPlayer::GetPlayTime()
{
    return libvlc_media_player_get_time(vlc_player_) / double(1000);
}

void VLCPlayer::SetFullScreen(bool istrue)
{
    libvlc_set_fullscreen(vlc_player_, istrue);
}
