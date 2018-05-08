#include "VLCListPlayer.h"

#include "vlc/vlc.h"

#include "VLCInstance.h"

VLCListPlayer::VLCListPlayer(VLCInstance* vlcInst)
    : vlc_inst_(vlcInst), media_list_(0), media_list_player_(0), media_player_(0)
{
    if (vlc_inst_)
    {
        media_list_player_ = libvlc_media_list_player_new(vlc_inst_->Get());
        media_player_ = libvlc_media_player_new(vlc_inst_->Get());

        libvlc_media_list_player_set_media_player(media_list_player_, media_player_);
        libvlc_media_list_player_set_playback_mode(media_list_player_, libvlc_playback_mode_repeat);
    }
}

VLCListPlayer::~VLCListPlayer()
{
    if (media_list_player_) libvlc_media_list_player_release(media_list_player_);
    if (media_player_) libvlc_media_player_release(media_player_);
}

void VLCListPlayer::SetRenderWindow(void* wndHandle)
{
    if (media_player_ && wndHandle)
    {
        libvlc_media_player_set_hwnd(media_player_, wndHandle);
    }
}

void VLCListPlayer::PlayList(const std::vector<std::string>& filelist)
{
    play_list_ = filelist;
    play_index_ = 0;
    media_list_ = libvlc_media_list_new(vlc_inst_->Get());
    for (std::vector<std::string>::const_iterator cit = filelist.begin();
        cit != filelist.end(); cit ++)
    {
        libvlc_media_t *vlc_media = libvlc_media_new_path(vlc_inst_->Get(), cit->c_str());
        int err = libvlc_media_list_add_media(media_list_, vlc_media);
        libvlc_media_release(vlc_media);
    }
    libvlc_media_list_player_set_media_list(media_list_player_, media_list_);
    libvlc_media_list_player_play(media_list_player_);
    if (media_list_) libvlc_media_list_release(media_list_);
}

void VLCListPlayer::PlayNext()
{
    play_index_++;
    if (play_index_ >= play_list_.size())
    {
        play_index_ = 0;
    }
    libvlc_media_list_player_play_item_at_index(media_list_player_, play_index_);
}
