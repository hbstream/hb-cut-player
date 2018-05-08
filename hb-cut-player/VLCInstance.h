#ifndef _VLC_INSTANCE_H_
#define _VLC_INSTANCE_H_

#include <string>

class libvlc_instance_t;

// VLCInstance 只允许建立一个对象

class VLCInstance
{
public:
    explicit VLCInstance(const std::string& pluginPath);

    ~VLCInstance();

    libvlc_instance_t* Get() { return vlc_inst_; }

    bool IsLoaded() const { return vlc_inst_ != 0; }

    std::string Version();

    void SetUserAgent(const std::string& appver, const std::string& httpver);

private:
    libvlc_instance_t* vlc_inst_;
};

#endif // _VLC_INSTANCE_H_
