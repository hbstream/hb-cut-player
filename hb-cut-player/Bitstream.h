#ifndef _BIT_STREAM_H_
#define _BIT_STREAM_H_

#include <memory>

typedef unsigned int hb_uint32;
typedef unsigned long long hb_uint64;

hb_uint32 BytesToUI32(const char* buf)
{
    return ( (((hb_uint32)buf[0]) << 24)	& 0xff000000 )
        | ( (((hb_uint32)buf[1]) << 16)	& 0xff0000 )
        | ( (((hb_uint32)buf[2]) << 8)	& 0xff00 )
        | ( (((hb_uint32)buf[3]))		& 0xff );
}

hb_uint64 BytesToUI64(const char* buf)
{
    return ( (((hb_uint64)buf[0]) << 56)	& 0xff00000000000000 )
        | ( (((hb_uint64)buf[1]) << 48)	& 0xff000000000000 )
        | ( (((hb_uint64)buf[2]) << 40)	& 0xff0000000000 )
        | ( (((hb_uint64)buf[3]) << 32)	& 0xff00000000 )
        | ( (((hb_uint64)buf[4]) << 24)	& 0xff000000 )
        | ( (((hb_uint64)buf[5]) << 16)	& 0xff0000 )
        | ( (((hb_uint64)buf[6]) << 8)	& 0xff00 )
        | ( (((hb_uint64)buf[7]))			& 0xff );
}

double BytesToDouble(const char* buf) 
{
    unsigned char b[8];
    std::memcpy(b, buf, 8);

    union {
        unsigned char dc[8];
        double dd;
    } d;

    d.dc[7] = b[0];
    d.dc[6] = b[1];
    d.dc[5] = b[2];
    d.dc[4] = b[3];
    d.dc[3] = b[4];
    d.dc[2] = b[5];
    d.dc[1] = b[6];
    d.dc[0] = b[7];

    return d.dd;
}

void UI32ToBytes(char* buf, hb_uint32 val)
{
    buf[0] = (char)(val >> 24) & 0xff;
    buf[1] = (char)(val >> 16) & 0xff;
    buf[2] = (char)(val >> 8) & 0xff;
    buf[3] = (char)(val) & 0xff;
}

void UI64ToBytes(char* buf, hb_uint64 val)
{
    buf[0] = (char)(val >> 56) & 0xff;
    buf[1] = (char)(val >> 48) & 0xff;
    buf[2] = (char)(val >> 40) & 0xff;
    buf[3] = (char)(val >> 32) & 0xff;
    buf[4] = (char)(val >> 24) & 0xff;
    buf[5] = (char)(val >> 16) & 0xff;
    buf[6] = (char)(val >> 8) & 0xff;
    buf[7] = (char)(val) & 0xff;
}

void DoubleToBytes(char* buf, double val) 
{
    union {
        unsigned char dc[8];
        double dd;
    } d;
    unsigned char b[8];

    d.dd = val;

    b[0] = d.dc[7];
    b[1] = d.dc[6];
    b[2] = d.dc[5];
    b[3] = d.dc[4];
    b[4] = d.dc[3];
    b[5] = d.dc[2];
    b[6] = d.dc[1];
    b[7] = d.dc[0];

    std::memcpy(buf, b, 8);
}

#endif // _BIT_STREAM_H_
