#include "VLCInstance.h"

#include "vlc/vlc.h"

VLCInstance::VLCInstance(const std::string& pluginPath)
{
    std::string plugin_arg = "--plugin-path=" + pluginPath;
    const char * const vlc_args[] = {
        "-I", "dummy", /* Don't use any interface */
        "--ignore-config", /* Don't use VLC's config */
        "--no-video-title",
        "--fullscreen",
        plugin_arg.c_str()
    };
    vlc_inst_ = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
}

VLCInstance::~VLCInstance()
{
    if (vlc_inst_)
    {
        libvlc_release(vlc_inst_);
    }
}

std::string VLCInstance::Version()
{
    return libvlc_get_version();
}

void VLCInstance::SetUserAgent(const std::string& appver, const std::string& httpver)
{
    if (vlc_inst_) libvlc_set_user_agent(vlc_inst_, appver.c_str(), httpver.c_str());
}
