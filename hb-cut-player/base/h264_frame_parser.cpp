/*******************************************************************************
 * h264_frame_parser.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2015-6-5 14:33 - Created (Haibin Du)
 ******************************************************************************/

#include "h264_frame_parser.h"

#include <climits>
#include <cmath>
#include <cstring>

#if !defined(INT_MAX)
#define INT_MAX 0x7FFFFFFF
#endif

#include "base/bit_stream.h"
#include "base/byte_stream.h"

const char* AVCFindStartCodeInternal(const char *p, const char *end)
{
    const char *a = p + 4 - ((intptr_t)p & 3);

    for (end -= 3; p < a && p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }

    for (end -= 3; p < end; p += 4) {
        unsigned int x = *(const unsigned int*)p;
        //      if ((x - 0x01000100) & (~x) & 0x80008000) // little endian
        //      if ((x - 0x00010001) & (~x) & 0x00800080) // big endian
        if ((x - 0x01010101) & (~x) & 0x80808080) { // generic
            if (p[1] == 0) {
                if (p[0] == 0 && p[2] == 1)
                    return p;
                if (p[2] == 0 && p[3] == 1)
                    return p+1;
            }
            if (p[3] == 0) {
                if (p[2] == 0 && p[4] == 1)
                    return p+2;
                if (p[4] == 0 && p[5] == 1)
                    return p+3;
            }
        }
    }

    for (end += 3; p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }

    return end + 3;
}

const char* AVCFindStartCode(const char *p, const char *end)
{
    const char *out= AVCFindStartCodeInternal(p, end);
    if(p<out && out<end && !out[-1]) out--;
    return out;
}

void AVCParseNalUnits(const char *bufIn, int inSize, char* bufOut, int* outSize)
{
    const char *p = bufIn;
    const char *end = p + inSize;
    const char *nal_start, *nal_end;

    char* pbuf = bufOut;

    *outSize = 0;
    nal_start = AVCFindStartCode(p, end);
    while (nal_start < end)
    {
        while(!*(nal_start++));

        nal_end = AVCFindStartCode(nal_start, end);

        unsigned int nal_size = nal_end - nal_start;
        pbuf = UI32ToBytes(pbuf, nal_size);
        memcpy(pbuf, nal_start, nal_size);
        pbuf += nal_size;

        nal_start = nal_end;
    }

    *outSize = (pbuf - bufOut);
}

void ParseH264Frame(const char* nalsbuf, int size, char* outBuf, int& outLen,
    char* spsBuf, int& spsSize, char* ppsBuf, int& ppsSize,
    bool& isKeyframe, int* pwidth, int* pheight)
{
    int tmp_len = 0;

    AVCParseNalUnits(nalsbuf, size, (char*)outBuf, &outLen);

    char* start = outBuf;
    char* end = outBuf + outLen;

    /* look for sps and pps */
    while (start < end) 
    {
        unsigned int size = BytesToUI32(start);
        unsigned char nal_type = start[4] & 0x1f;

        if (nal_type == 7 && spsBuf)        /* SPS */
        {
            spsSize = size;
            memcpy(spsBuf, start + 4, spsSize);

            ff_h264_decode_sps(start+4, size, pwidth, pheight);
        }
        else if (nal_type == 8 && ppsBuf)   /* PPS */
        {
            ppsSize = size;
            memcpy(ppsBuf, start + 4, ppsSize);
        }
        else if (nal_type == 5)
        {
            isKeyframe = true;
        }
        start += size + 4;
    }
}

void AVCParseNalUnitsNoCopy(char *bufIn, int inSize)
{
    char *p = bufIn;
    char *end = p + inSize;
    const char *nal_start, *nal_end;

    nal_start = AVCFindStartCode(p, end);
    while (nal_start < end)
    {
        while(!*(nal_start++));

        nal_end = AVCFindStartCode(nal_start, end);

        unsigned int nal_size = nal_end - nal_start;
        UI32ToBytes((char*)nal_start-4, nal_size);

        nal_start = nal_end;
    }
}

void ParseH264FrameNoCopy(char* nalsbuf, int size,
    char* spsBuf, int* spsSize, char* ppsBuf, int* ppsSize,
    bool& isKeyframe)
{
    int tmp_len = 0;

    AVCParseNalUnitsNoCopy(nalsbuf, size);

    const char* start = nalsbuf;
    const char* end = start + size;

    /* look for sps and pps */
    while (start < end) 
    {
        unsigned int size = BytesToUI32(start);
        unsigned char nal_type = start[4] & 0x1f;

        if (nal_type == 7 && spsBuf)        /* SPS */
        {
            *spsSize = size;
            memcpy(spsBuf, start + 4, *spsSize);
        }
        else if (nal_type == 8 && ppsBuf)   /* PPS */
        {
            *ppsSize = size;
            memcpy(ppsBuf, start + 4, *ppsSize);
        }
        else if (nal_type == 5)
        {
            isKeyframe = true;
        }
        start += size + 4;
    }
}

bool CheckH264Frame(const char* frameBuf, int bufLen,
    char* spsBuf, int* spsSize, char* ppsBuf, int* ppsSize,
    bool& isKeyframe, int* firstMbInSliceIsZero)
{
    const char *p = frameBuf;
    const char *end = p + bufLen;
    const char *nal_start, *nal_end;

    bool is_has_sps = false;
	if (firstMbInSliceIsZero)
    {
        *firstMbInSliceIsZero = -1;
    }

    nal_start = AVCFindStartCode(p, end);
    while (nal_start < end)
    {
        while(!*(nal_start++));

        nal_end = AVCFindStartCode(nal_start, end);

        unsigned int nal_size = nal_end - nal_start;
        unsigned char nal_type = nal_start[0] & 0x1f;

        if (nal_type == 7 && spsBuf)        /* SPS */
        {
            *spsSize = nal_size;
            memcpy(spsBuf, nal_start, *spsSize);

            is_has_sps = true;
        }
        else if (nal_type == 8 && ppsBuf)   /* PPS */
        {
            *ppsSize = nal_size;
            memcpy(ppsBuf, nal_start, *ppsSize);
        }
        else if (nal_type == 5)
        {
            isKeyframe = true;
        }

        if (firstMbInSliceIsZero && (nal_type == 5 || nal_type == 1))
        {
            if (*firstMbInSliceIsZero == -1)
            {
                *firstMbInSliceIsZero = (nal_start[1] & 0x80) ? 1 : 0;
            }
        }

        nal_start = nal_end;
    }

    return is_has_sps;
}



namespace {

// TODO: 这里可以参照ffmpeg进行ExpG(指数哥伦布码)编码优化

inline int vlc_bs_read_ue(BitReader& bitReader)
{
    int i = 0;
    while (bitReader.ReadBit1() == 0 && i < 32)
    {
        i++;
    }
    return( ( 1 << i) - 1 + bitReader.ReadBitUI32(i) );
}

inline int vlc_bs_read_se(BitReader& bitReader)
{
    int val = vlc_bs_read_ue(bitReader);

    return (val & 0x01) ? (val+1)/2 : -(val/2);
}

}

/**
 * rational number numerator/denominator
 */
struct FFFmpegRational
{
    int num; ///< numerator
    int den; ///< denominator
};

#define MAX_SPS_COUNT 32
#define MAX_PPS_COUNT 256
#define MAX_PICTURE_COUNT 32

#if !defined(FFMPEG_ARRAY_ELEMS)
#define FFMPEG_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))
#endif

enum FFmpegAVColorPrimaries{
    AVCOL_PRI_BT709       = 1, ///< also ITU-R BT1361 / IEC 61966-2-4 / SMPTE RP177 Annex B
    AVCOL_PRI_UNSPECIFIED = 2,
    AVCOL_PRI_BT470M      = 4,
    AVCOL_PRI_BT470BG     = 5, ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM
    AVCOL_PRI_SMPTE170M   = 6, ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC
    AVCOL_PRI_SMPTE240M   = 7, ///< functionally identical to above
    AVCOL_PRI_FILM        = 8,
    AVCOL_PRI_BT2020      = 9, ///< ITU-R BT2020
    AVCOL_PRI_NB             , ///< Not part of ABI
};

enum FFmpegAVColorTransferCharacteristic{
    AVCOL_TRC_BT709        =  1, ///< also ITU-R BT1361
    AVCOL_TRC_UNSPECIFIED  =  2,
    AVCOL_TRC_GAMMA22      =  4, ///< also ITU-R BT470M / ITU-R BT1700 625 PAL & SECAM
    AVCOL_TRC_GAMMA28      =  5, ///< also ITU-R BT470BG
    AVCOL_TRC_SMPTE170M    =  6, ///< also ITU-R BT601-6 525 or 625 / ITU-R BT1358 525 or 625 / ITU-R BT1700 NTSC
    AVCOL_TRC_SMPTE240M    =  7,
    AVCOL_TRC_LINEAR       =  8, ///< "Linear transfer characteristics"
    AVCOL_TRC_LOG          =  9, ///< "Logarithmic transfer characteristic (100:1 range)"
    AVCOL_TRC_LOG_SQRT     = 10, ///< "Logarithmic transfer characteristic (100 * Sqrt( 10 ) : 1 range)"
    AVCOL_TRC_IEC61966_2_4 = 11, ///< IEC 61966-2-4
    AVCOL_TRC_BT1361_ECG   = 12, ///< ITU-R BT1361 Extended Colour Gamut
    AVCOL_TRC_IEC61966_2_1 = 13, ///< IEC 61966-2-1 (sRGB or sYCC)
    AVCOL_TRC_BT2020_10    = 14, ///< ITU-R BT2020 for 10 bit system
    AVCOL_TRC_BT2020_12    = 15, ///< ITU-R BT2020 for 12 bit system
    AVCOL_TRC_NB               , ///< Not part of ABI
};


enum FFmpegAVColorSpace{
    AVCOL_SPC_RGB         =  0,
    AVCOL_SPC_BT709       =  1, ///< also ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
    AVCOL_SPC_UNSPECIFIED =  2,
    AVCOL_SPC_FCC         =  4,
    AVCOL_SPC_BT470BG     =  5, ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM / IEC 61966-2-4 xvYCC601
    AVCOL_SPC_SMPTE170M   =  6, ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC / functionally identical to above
    AVCOL_SPC_SMPTE240M   =  7,
    AVCOL_SPC_YCOCG       =  8, ///< Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16
    AVCOL_SPC_BT2020_NCL  =  9, ///< ITU-R BT2020 non-constant luminance system
    AVCOL_SPC_BT2020_CL   = 10, ///< ITU-R BT2020 constant luminance system
    AVCOL_SPC_NB              , ///< Not part of ABI
};

/**
 * Sequence parameter set
 */
struct FFmpegSps
{
    int profile_idc;
    int level_idc;
    int chroma_format_idc;
    int transform_bypass;              ///< qpprime_y_zero_transform_bypass_flag
    int log2_max_frame_num;            ///< log2_max_frame_num_minus4 + 4
    int poc_type;                      ///< pic_order_cnt_type
    int log2_max_poc_lsb;              ///< log2_max_pic_order_cnt_lsb_minus4
    int delta_pic_order_always_zero_flag;
    int offset_for_non_ref_pic;
    int offset_for_top_to_bottom_field;
    int poc_cycle_length;              ///< num_ref_frames_in_pic_order_cnt_cycle
    int ref_frame_count;               ///< num_ref_frames
    int gaps_in_frame_num_allowed_flag;
    int mb_width;                      ///< pic_width_in_mbs_minus1 + 1
    int mb_height;                     ///< pic_height_in_map_units_minus1 + 1
    int frame_mbs_only_flag;
    int mb_aff;                        ///<mb_adaptive_frame_field_flag
    int direct_8x8_inference_flag;
    int crop;                   ///< frame_cropping_flag
    unsigned int crop_left;            ///< frame_cropping_rect_left_offset
    unsigned int crop_right;           ///< frame_cropping_rect_right_offset
    unsigned int crop_top;             ///< frame_cropping_rect_top_offset
    unsigned int crop_bottom;          ///< frame_cropping_rect_bottom_offset
    int vui_parameters_present_flag;
    FFFmpegRational sar;
    int video_signal_type_present_flag;
    int full_range;
    int colour_description_present_flag;
    enum FFmpegAVColorPrimaries color_primaries;
    enum FFmpegAVColorTransferCharacteristic color_trc;
    enum FFmpegAVColorSpace colorspace;
    int timing_info_present_flag;
    hb_uint32 num_units_in_tick;
    hb_uint32 time_scale;
    int fixed_frame_rate_flag;
    short offset_for_ref_frame[256]; //FIXME dyn aloc?
    int bitstream_restriction_flag;
    int num_reorder_frames;
    int scaling_matrix_present;
    hb_uint8 scaling_matrix4[6][16];
    hb_uint8 scaling_matrix8[6][64];
    int nal_hrd_parameters_present_flag;
    int vcl_hrd_parameters_present_flag;
    int pic_struct_present_flag;
    int time_offset_length;
    int cpb_cnt;                       ///< See H.264 E.1.2
    int initial_cpb_removal_delay_length; ///< initial_cpb_removal_delay_length_minus1 +1
    int cpb_removal_delay_length;      ///< cpb_removal_delay_length_minus1 + 1
    int dpb_output_delay_length;       ///< dpb_output_delay_length_minus1 + 1
    int bit_depth_luma;                ///< bit_depth_luma_minus8 + 8
    int bit_depth_chroma;              ///< bit_depth_chroma_minus8 + 8
    int residual_color_transform_flag; ///< residual_colour_transform_flag
    int constraint_set_flags;          ///< constraint_set[0-3]_flag
};

int ffmpeg_image_check_size(unsigned int w, unsigned int h)
{
    if ((int)w>0 && (int)h>0 && (w+128)*(hb_uint64)(h+128) < INT_MAX/8)
        return 0;

    //av_log(&imgutils, AV_LOG_ERROR, "Picture size %ux%u is invalid\n", w, h);
    return 1;
}

static const hb_uint8 ffmpeg_zigzag_scan[16]= {
    0+0*4, 1+0*4, 0+1*4, 0+2*4,
    1+1*4, 2+0*4, 3+0*4, 2+1*4,
    1+2*4, 0+3*4, 1+3*4, 2+2*4,
    3+1*4, 3+2*4, 2+3*4, 3+3*4,
};

const hb_uint8 ffmpeg_zigzag_direct[64] = {
    0,   1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

void decode_scaling_list(BitReader& bitReader, 
    hb_uint8 *factors, int size,
    const hb_uint8 *jvt_list, const hb_uint8 *fallback_list)
{
    int i, last = 8, next = 8;
    const hb_uint8 *scan = 
        (size == 16) ? ffmpeg_zigzag_scan : ffmpeg_zigzag_direct;
    if(!bitReader.ReadBit1()) /* matrix not written, we use the predicted one */
    {
        memcpy(factors, fallback_list, size*sizeof(hb_uint8));
    }
    else
    {
        for(i=0;i<size;i++)
        {
            if(next)
                next = (last + vlc_bs_read_se(bitReader)) & 0xff;
            if(!i && !next)
            { /* matrix not written, we use the preset one */
                memcpy(factors, jvt_list, size*sizeof(hb_uint8));
                break;
            }
            last = factors[scan[i]] = next ? next : last;
        }
    }
}

static const hb_uint8 ffmpeg_default_scaling4[2][16]={
    {   6,13,20,28,
    13,20,28,32,
    20,28,32,37,
    28,32,37,42
    },{
        10,14,20,24,
            14,20,24,27,
            20,24,27,30,
            24,27,30,34
    }};

    static const hb_uint8 ffmpeg_default_scaling8[2][64]={
        {   6,10,13,16,18,23,25,27,
        10,11,16,18,23,25,27,29,
        13,16,18,23,25,27,29,31,
        16,18,23,25,27,29,31,33,
        18,23,25,27,29,31,33,36,
        23,25,27,29,31,33,36,38,
        25,27,29,31,33,36,38,40,
        27,29,31,33,36,38,40,42
        },{
            9,13,15,17,19,21,22,24,
                13,13,17,19,21,22,24,25,
                15,17,19,21,22,24,25,27,
                17,19,21,22,24,25,27,28,
                19,21,22,24,25,27,28,30,
                21,22,24,25,27,28,30,32,
                22,24,25,27,28,30,32,33,
                24,25,27,28,30,32,33,35
        }};

void ffmpeg_decode_scaling_matrices(BitReader& bitReader, FFmpegSps* sps, int is_sps,
    hb_uint8 (*scaling_matrix4)[16], hb_uint8 (*scaling_matrix8)[64])
{
        int fallback_sps = !is_sps && sps->scaling_matrix_present;
        const hb_uint8 *fallback[4] = {
            fallback_sps ? sps->scaling_matrix4[0] : ffmpeg_default_scaling4[0],
            fallback_sps ? sps->scaling_matrix4[3] : ffmpeg_default_scaling4[1],
            fallback_sps ? sps->scaling_matrix8[0] : ffmpeg_default_scaling8[0],
            fallback_sps ? sps->scaling_matrix8[3] : ffmpeg_default_scaling8[1]
        };

        if(bitReader.ReadBit1())
        {
            sps->scaling_matrix_present |= is_sps;
            // Intra, Y
            decode_scaling_list(bitReader,scaling_matrix4[0],16,
                ffmpeg_default_scaling4[0],fallback[0]);
            // Intra, Cr
            decode_scaling_list(bitReader,scaling_matrix4[1],16,
                ffmpeg_default_scaling4[0],scaling_matrix4[0]);
            // Intra, Cb
            decode_scaling_list(bitReader,scaling_matrix4[2],16,
                ffmpeg_default_scaling4[0],scaling_matrix4[1]);
            // Inter, Y
            decode_scaling_list(bitReader,scaling_matrix4[3],16,
                ffmpeg_default_scaling4[1],fallback[1]);
            // Inter, Cr
            decode_scaling_list(bitReader,scaling_matrix4[4],16,
                ffmpeg_default_scaling4[1],scaling_matrix4[3]);
            // Inter, Cb
            decode_scaling_list(bitReader,scaling_matrix4[5],16,
                ffmpeg_default_scaling4[1],scaling_matrix4[4]);

            if(is_sps/* || pps->transform_8x8_mode*/)
            {
                // Intra, Y
                decode_scaling_list(bitReader,scaling_matrix8[0],64,
                    ffmpeg_default_scaling8[0],fallback[2]);
                if(sps->chroma_format_idc == 3)
                {
                    // Intra, Cr
                    decode_scaling_list(bitReader,scaling_matrix8[1],64,
                        ffmpeg_default_scaling8[0],scaling_matrix8[0]);
                    // Intra, Cb
                    decode_scaling_list(bitReader,scaling_matrix8[2],64,
                        ffmpeg_default_scaling8[0],scaling_matrix8[1]);
                }
                // Inter, Y
                decode_scaling_list(bitReader,scaling_matrix8[3],64,
                    ffmpeg_default_scaling8[1],fallback[3]);
                if(sps->chroma_format_idc == 3)
                {
                    // Inter, Cr
                    decode_scaling_list(bitReader,scaling_matrix8[4],64,
                        ffmpeg_default_scaling8[1],scaling_matrix8[3]);
                    // Inter, Cb
                    decode_scaling_list(bitReader,scaling_matrix8[5],64,
                        ffmpeg_default_scaling8[1],scaling_matrix8[4]);
                }
            }
        }
}

/***
 * 参照ffmpeg libavcodec/h264_ps.c, ff_h264_decode_seq_parameter_set函数
 */
int ff_h264_decode_sps(const char* spsBuf, int spsLen, int* pwidth, int* pheight)
{
    if (!pwidth || !pheight) return -1;

    BitReader bit_reader(spsBuf);

    int mb_width = 0;
    int mb_height = 0;
    int chroma_y_shift = 0; // 400 uses yuv420p

    // NAL Unit Type, not part of sps
    int forbidden_zero_bit = bit_reader.ReadBit1();
    int nal_ref_idc = bit_reader.ReadBitUI32(2);
    int nal_unit_type = bit_reader.ReadBitUI32(5);

    int profile_idc= bit_reader.ReadBitUI32(8);
    int constraint_set_flags = 0;
    constraint_set_flags |= bit_reader.ReadBit1() << 0;   //constraint_set0_flag
    constraint_set_flags |= bit_reader.ReadBit1() << 1;   //constraint_set1_flag
    constraint_set_flags |= bit_reader.ReadBit1() << 2;   //constraint_set2_flag
    constraint_set_flags |= bit_reader.ReadBit1() << 3;   //constraint_set3_flag
    bit_reader.SkipBits(4); // reserved

    int level_idc= bit_reader.ReadBitUI32(8);
    unsigned int sps_id = vlc_bs_read_ue(bit_reader);

    if(sps_id >= MAX_SPS_COUNT)
    {
        return -1;
    }

    FFmpegSps* sps = new FFmpegSps;
    if(sps == NULL) return -1;

    sps->time_offset_length = 24;
    sps->profile_idc = profile_idc;
    sps->constraint_set_flags = constraint_set_flags;
    sps->level_idc = level_idc;

    memset(sps->scaling_matrix4, 16, sizeof(sps->scaling_matrix4));
    memset(sps->scaling_matrix8, 16, sizeof(sps->scaling_matrix8));
    sps->scaling_matrix_present = 0;

    if(sps->profile_idc >= 100) // high profile
    {
        sps->chroma_format_idc= vlc_bs_read_ue(bit_reader);
        if(sps->chroma_format_idc == 3)
            sps->residual_color_transform_flag = bit_reader.ReadBit1();
        sps->bit_depth_luma   = vlc_bs_read_ue(bit_reader) + 8;
        sps->bit_depth_chroma = vlc_bs_read_ue(bit_reader) + 8;
        sps->transform_bypass = bit_reader.ReadBit1();
        ffmpeg_decode_scaling_matrices(bit_reader, sps, 1, 
            sps->scaling_matrix4, sps->scaling_matrix8);
    }
    else
    {
        sps->chroma_format_idc= 1;
        sps->bit_depth_luma   = 8;
        sps->bit_depth_chroma = 8;
    }

    sps->log2_max_frame_num= vlc_bs_read_ue(bit_reader) + 4;
    sps->poc_type= vlc_bs_read_ue(bit_reader);

    if(sps->poc_type == 0)
    {
        sps->log2_max_poc_lsb= vlc_bs_read_ue(bit_reader) + 4;
    }
    else if(sps->poc_type == 1)
    {
        sps->delta_pic_order_always_zero_flag = bit_reader.ReadBit1();
        sps->offset_for_non_ref_pic           = vlc_bs_read_se(bit_reader);
        sps->offset_for_top_to_bottom_field   = vlc_bs_read_se(bit_reader);
        sps->poc_cycle_length                 = vlc_bs_read_ue(bit_reader);

        if((unsigned)sps->poc_cycle_length >= 
            FFMPEG_ARRAY_ELEMS(sps->offset_for_ref_frame))
        {
            //av_log(h->s.avctx, AV_LOG_ERROR, "poc_cycle_length overflow %u\n", sps->poc_cycle_length);
            goto fail;
        }

        for(int i=0; i<sps->poc_cycle_length; i++)
            sps->offset_for_ref_frame[i]= vlc_bs_read_se(bit_reader);
    }
    else if(sps->poc_type != 2)
    {
        //av_log(h->s.avctx, AV_LOG_ERROR, "illegal POC type %d\n", sps->poc_type);
        goto fail;
    }

    sps->ref_frame_count= vlc_bs_read_ue(bit_reader);
    if(sps->ref_frame_count > MAX_PICTURE_COUNT-2 || sps->ref_frame_count >= 32U)
    {
        //av_log(h->s.avctx, AV_LOG_ERROR, "too many reference frames\n");
        goto fail;
    }
    sps->gaps_in_frame_num_allowed_flag= bit_reader.ReadBit1();
    sps->mb_width = vlc_bs_read_ue(bit_reader) + 1;
    sps->mb_height= vlc_bs_read_ue(bit_reader) + 1;

//     width  = 16 * sps->mb_width;
//     height = 16 * sps->mb_height;

    if((unsigned)sps->mb_width >= INT_MAX/16 ||
        (unsigned)sps->mb_height >= INT_MAX/16 ||
       ffmpeg_image_check_size(16*sps->mb_width, 16*sps->mb_height))
    {
        //av_log(h->s.avctx, AV_LOG_ERROR, "mb_width/height overflow\n");
        goto fail;
    }
//    return 0;

    //////////////////////////////////////////////////////////////////////////

    sps->frame_mbs_only_flag= bit_reader.ReadBit1();
    if(!sps->frame_mbs_only_flag)
        sps->mb_aff= bit_reader.ReadBit1();
    else
        sps->mb_aff= 0;

    sps->direct_8x8_inference_flag= bit_reader.ReadBit1();
    if(!sps->frame_mbs_only_flag && !sps->direct_8x8_inference_flag)
    {
        //av_log(h->s.avctx, AV_LOG_ERROR, "This stream was generated by a broken encoder, invalid 8x8 inference\n");
        goto fail;
    }

#ifndef ALLOW_INTERLACE
//     if(sps->mb_aff)
//         av_log(h->s.avctx, AV_LOG_ERROR, "MBAFF support not included; enable it at compile-time.\n");
#endif
    sps->crop=bit_reader.ReadBit1();
    if(sps->crop)
    {
        int crop_vertical_limit = sps->chroma_format_idc & 2 ? 16 : 8;
        int crop_horizontal_limit = sps->chroma_format_idc == 3 ? 16 : 8;
        sps->crop_left  = vlc_bs_read_ue(bit_reader);
        sps->crop_right = vlc_bs_read_ue(bit_reader);
        sps->crop_top   = vlc_bs_read_ue(bit_reader);
        sps->crop_bottom= vlc_bs_read_ue(bit_reader);
        if(sps->crop_left || sps->crop_top)
        {
            //av_log(h->s.avctx, AV_LOG_ERROR, "insane cropping not completely supported, this could look slightly wrong ... (left: %d, top: %d)\n", sps->crop_left, sps->crop_top);
        }
        if(sps->crop_right >= crop_horizontal_limit ||
            sps->crop_bottom >= crop_vertical_limit)
        {
            //av_log(h->s.avctx, AV_LOG_ERROR, "brainfart cropping not supported, cropping disabled (right: %d, bottom: %d)\n", sps->crop_right, sps->crop_bottom);
        /* It is very unlikely that partial cropping will make anybody happy.
         * Not cropping at all fixes for example playback of Sisvel 3D streams
         * in applications supporting Sisvel 3D. */
        sps->crop_left  =
        sps->crop_right =
        sps->crop_top   =
        sps->crop_bottom= 0;
        }
    }
    else
    {
        sps->crop_left  =
        sps->crop_right =
        sps->crop_top   =
        sps->crop_bottom= 0;
    }

    sps->vui_parameters_present_flag= bit_reader.ReadBit1();
    if( sps->vui_parameters_present_flag )
    {
//         if (decode_vui_parameters(h, sps) < 0)
//             goto fail;
    }

    if(!sps->sar.den)
        sps->sar.den= 1;

    // 开始计算

    mb_width = sps->mb_width;
    mb_height = sps->mb_height * (2 - sps->frame_mbs_only_flag);
    chroma_y_shift = sps->chroma_format_idc <= 1; // 400 uses yuv420p

#define CHROMA444 (sps->chroma_format_idc == 3)
#define OUR_FFMIN(a,b) ((a) > (b) ? (b) : (a))

    *pwidth = 16*mb_width - (2>>CHROMA444)*OUR_FFMIN(sps->crop_right, (8<<CHROMA444)-1);
    if(sps->frame_mbs_only_flag)
        *pheight= 16*mb_height - (1<<chroma_y_shift)*OUR_FFMIN(sps->crop_bottom, (16>>chroma_y_shift)-1);
    else
        *pheight= 16*mb_height - (2<<chroma_y_shift)*OUR_FFMIN(sps->crop_bottom, (16>>chroma_y_shift)-1);

    delete sps;
    return 0;
fail:
    delete sps;
    return -1;
}

bool GetWidthHeightFromFrame(const char* frameBuf, int bufLen, int& width, int& height,
    char* spsBuf, int& spsSize, char* ppsBuf, int& ppsSize)
{
    const char *p = frameBuf;
    const char *end = p + bufLen;
    const char *nal_start, *nal_end;

    nal_start = AVCFindStartCode(p, end);
    while (nal_start < end)
    {
        while(!*(nal_start++));

        nal_end = AVCFindStartCode(nal_start, end);

        unsigned int nal_size = nal_end - nal_start;
        unsigned char nal_type = nal_start[0] & 0x1f;

        if (nal_type == 7)  // sps buf
        {
            if (spsBuf)
            {
                spsSize = nal_size;
                memcpy(spsBuf, nal_start, spsSize);
            }
            ff_h264_decode_sps(nal_start, nal_size, &width, &height);
        }
        else if (nal_type == 8)   /* PPS */
        {
            ppsSize = nal_size;
            memcpy(ppsBuf, nal_start, ppsSize);
        }

        nal_start = nal_end;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
// class H264FirstMbSliceBuffering

void H264FirstMbSliceBuffering::AppendBuffer(const char* h264Buf, int bufSize,
    long long timestamp)
{
    bool is_key = false;
    int firstMbInSliceIsZero;
    CheckH264Frame(h264Buf, bufSize, NULL, NULL, NULL, NULL, is_key,
        &firstMbInSliceIsZero);
    if (firstMbInSliceIsZero == 1)
    {
        if (h264buffer_.size())
        {
            observer_->OnH264SliceCompleteFrame(h264buffer_.data(),
                h264buffer_.size(), h264buffer_timestamp_);
            h264buffer_.resize(0);
        }

        h264buffer_.resize(bufSize);
        memcpy(&h264buffer_[0], h264Buf, bufSize);
        h264buffer_timestamp_ = timestamp;
    }
    else
    {
        int last_size = h264buffer_.size();
        h264buffer_.resize(last_size + bufSize);
        memcpy(&h264buffer_[last_size], h264Buf, bufSize);
    }
}
