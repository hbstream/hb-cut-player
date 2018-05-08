/*******************************************************************************
 * h264_frame_parser.h
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 *
 *
 * -----------------------------------------------------------------------------
 * 2015-6-5 14:33 - Created (Haibin Du)
 ******************************************************************************/

#ifndef _HDEV_H264_FRAME_PARSER_H_
#define _HDEV_H264_FRAME_PARSER_H_

#include "base/base.h"

#include <vector>

const char* AVCFindStartCodeInternal(const char *p, const char *end);

const char* AVCFindStartCode(const char *p, const char *end);

void AVCParseNalUnits(const char *bufIn, int inSize, char* bufOut, int* outSize);

void ParseH264Frame(const char* nalsbuf, int size, char* outBuf, int& outLen,
    char* spsBuf, int& spsSize, char* ppsBuf, int& ppsSize,
    bool& isKeyframe, int* pwidth, int* pheight);
	
void ParseH264FrameNoCopy(char* nalsbuf, int size,
    char* spsBuf, int* spsSize, char* ppsBuf, int* ppsSize,
    bool& isKeyframe);

bool CheckH264Frame(const char* frameBuf, int bufLen,
    char* spsBuf, int* spsSize, char* ppsBuf, int* ppsSize,
    bool& isKeyframe, int* firstMbInSliceIsZero = NULL);

int ff_h264_decode_sps(const char* spsBuf, int spsLen,
    int* pwidth, int* pheight);

bool GetWidthHeightFromFrame(const char* frameBuf, int bufLen, int& width, int& height,
    char* spsBuf, int& spsSize, char* ppsBuf, int& ppsSize);

class H264FirstMbSliceBuffering
{
public:
    class Observer
    {
    public:
        virtual ~Observer() {}

        virtual void OnH264SliceCompleteFrame(const char* frameBuf,
            int frameSize, long long timestamp) = 0;
    };

public:
    explicit H264FirstMbSliceBuffering(Observer* ob)
    {
        observer_ = ob;
        h264buffer_timestamp_ = 0;
    }

    void AppendBuffer(const char* h264Buf, int bufSize, long long timestamp);

private:
    Observer* observer_;
    std::vector<char> h264buffer_;
    long long h264buffer_timestamp_;
};

#endif // _HDEV_H264_FRAME_PARSER_H_
