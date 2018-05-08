/*******************************************************************************
 * bit_stream.cpp
 * Copyright: (c) 2013 Haibin Du(haibinnet@qq.com). All rights reserved.
 *
 * -----------------------------------------------------------------------------
 * 2013-5-15 - Created (Haibin Du)
 ******************************************************************************/

#include "bit_stream.h"

hb_uint8 BitReader::ShowBitUI08(int bitN)
{
    int last_byte_pos = BytePos();
    int last_bit_pos = BitPos();

    hb_uint8 val = 0;

    for (int i = 0; i != bitN; ++i)
    {
        val <<= 1;
        val |= ReadBit1();
    }

    SetBytePos(last_byte_pos);
    SetBitPos(last_bit_pos);

    return val;
}

hb_uint16 BitReader::ShowBitUI16(int bitN)
{
    int last_byte_pos = BytePos();
    int last_bit_pos = BitPos();

    hb_uint16 val = 0;

    for (int i = 0; i != bitN; ++i)
    {
        val <<= 1;
        val |= ReadBit1();
    }

    SetBytePos(last_byte_pos);
    SetBitPos(last_bit_pos);

    return val;
}

hb_uint32 BitReader::ShowBitUI32(int bitN)
{
    int last_byte_pos = BytePos();
    int last_bit_pos = BitPos();

    hb_uint32 val = 0;

    for (int i = 0; i != bitN; ++i)
    {
        val <<= 1;
        val |= ReadBit1();
    }

    SetBytePos(last_byte_pos);
    SetBitPos(last_bit_pos);

    return val;
}

hb_uint64 BitReader::ShowBitUI64(int bitN)
{
    int last_byte_pos = BytePos();
    int last_bit_pos = BitPos();

    hb_uint64 val = 0;

    for (int i = 0; i != bitN; ++i)
    {
        val <<= 1;
        val |= ReadBit1();
    }

    SetBytePos(last_byte_pos);
    SetBitPos(last_bit_pos);

    return val;
}
