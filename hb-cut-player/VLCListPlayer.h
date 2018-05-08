#ifndef _VLC_LIST_PLAYER_H_
#define _VLC_LIST_PLAYER_H_

#include <vector>
#include <string>

#include "vlc/libvlc_structures.h"

class libvlc_media_list_t;
class libvlc_media_list_player_t;
class libvlc_media_player_t;
class VLCInstance;

class VLCListPlayer
{
public:
    VLCListPlayer(VLCInstance* vlcInst);

    ~VLCListPlayer();

    void SetRenderWindow(void* wndHandle);

    void PlayList(const std::vector<std::string>& filelist);

    void PlayNext();

private:
    libvlc_media_list_t *media_list_;
    libvlc_media_list_player_t *media_list_player_;
    libvlc_media_player_t *media_player_;
    VLCInstance* vlc_inst_;

    std::vector<std::string> play_list_;
    int play_index_;
};

#endif // _VLC_LIST_PLAYER_H_
