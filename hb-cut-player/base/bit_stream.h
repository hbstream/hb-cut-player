/*******************************************************************************
 * bit_stream.h
 * Copyright: (c) 2013 Haibin Du(haibinnet@qq.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 *
 *
 * -----------------------------------------------------------------------------
 * 2013-5-15 - Created (Haibin Du)
 ******************************************************************************/

#ifndef _HBASE_BIT_STREAM_h_
#define _HBASE_BIT_STREAM_h_

#include "base/base.h"
#include "base/typedefs.h"

// ----------------------------------------------------------------------------
// BitReader
// ----------------------------------------------------------------------------

class BitReader
{
public:
    explicit BitReader(const char* srcBuf, int startByte = 0, int startBit = 0)
        : src_buf_(srcBuf), byte_pos_(0), bit_pos_(0)
    {

    }

    BitReader(const BitReader& other)
    {
        this->src_buf_ = other.src_buf_;
        this->byte_pos_ = other.byte_pos_;
        this->bit_pos_ = other.bit_pos_;
    }

    const BitReader& operator=(const BitReader& other)
    {
        this->src_buf_ = other.src_buf_;
        this->byte_pos_ = other.byte_pos_;
        this->bit_pos_ = other.bit_pos_;
        return *this;
    }

    const char* SrcBuf() const { return src_buf_; }

    void SetBytePos(int bytePos) { byte_pos_ = bytePos; }

    int BytePos() const { return byte_pos_; }

    void SetBitPos(int bitPos) { bit_pos_ = bitPos; }

    int BitPos() const { return bit_pos_; }

    void Align()
    {
        if (bit_pos_ != 0)
        {
            byte_pos_++;
            bit_pos_ = 0;
        }
    }

    void SkipBits(int bitN)
    {
        for (int i = 0; i != bitN; ++i)
        {
            bit_pos_++;
            if (bit_pos_ == 8)
            {
                byte_pos_++;
                bit_pos_ = 0;
            }
        }
    }

    void SkipBytes(int byteN)
    {
        byte_pos_ += byteN;
    }

    hb_uint8 ReadBit1()
    {
        hb_uint8 val = 0;
        int shift_len = 7 - bit_pos_;
        if (shift_len)
        {
            val = (src_buf_[byte_pos_] >> shift_len) & 0x01;
        }
        else
        {
            val = src_buf_[byte_pos_] & 0x01;
        }
        bit_pos_++;
        if (bit_pos_ == 8)
        {
            byte_pos_++;
            bit_pos_ = 0;
        }
        return val;
    }

    hb_uint8 ReadBitUI08(int bitN)
    {
        hb_uint8 val = 0;

        for (int i = 0; i != bitN; ++i)
        {
            val <<= 1;
            val |= ReadBit1();
        }

        return val;
    }

    hb_uint16 ReadBitUI16(int bitN)
    {
        hb_uint16 val = 0;

        for (int i = 0; i != bitN; ++i)
        {
            val <<= 1;
            val |= ReadBit1();
        }

        return val;
    }

    hb_uint32 ReadBitUI32(int bitN)
    {
        hb_uint32 val = 0;

        for (int i = 0; i != bitN; ++i)
        {
            val <<= 1;
            val |= ReadBit1();
        }

        return val;
    }

    hb_uint64 ReadBitUI64(int bitN)
    {
        hb_uint64 val = 0;

        for (int i = 0; i != bitN; ++i)
        {
            val <<= 1;
            val |= ReadBit1();
        }

        return val;
    }

    hb_uint8 ShowBitUI08(int bitN);

    hb_uint16 ShowBitUI16(int bitN);

    hb_uint32 ShowBitUI32(int bitN);

    hb_uint64 ShowBitUI64(int bitN);

private:
    const char* src_buf_;
    int byte_pos_;
    int bit_pos_;
};

#endif // _HBASE_BIT_STREAM_h_
