#ifndef _VLC_PLAYER_H_
#define _VLC_PLAYER_H_

class libvlc_media_t;
class libvlc_media_player_t;
class VLCInstance;

#include "vlc/libvlc_structures.h"

class VLCPlayer
{
public:
    VLCPlayer(VLCInstance* vlcInst);

    ~VLCPlayer();

    void SetRenderWindow(void* wndHandle);

    void PlayUrl(const char* url);

    void PlayFile(const char* filepath);

    void Stop();

    void Pause();

    void SetVolume(int volume);

    int GetVolume();

    void SetPlayRate(float rate);

    float GetPlayRate();

    double GetDuration();

    void SetPlayTime(double seekTime);

    double GetPlayTime();

    void SetFullScreen(bool istrue);

    bool IsPlaying();

private:
    libvlc_time_t duration_;

    libvlc_media_t* vlc_media_;
    libvlc_media_player_t* vlc_player_;
    VLCInstance* vlc_inst_;
    bool is_playing_;
};

#endif // _VLC_PLAYER_H_
