#include "byte_stream.h"

#include <cstring>

hb_uint8 BytesToUI08(const char* buf)
{
    return buf[0];
}

hb_uint8 BytesToUI08LE(const char* buf)
{
    return buf[0];
}

hb_uint16 BytesToUI16(const char* buf)
{
    return ( (((hb_uint16)buf[0]) << 8 )	& 0xff00 )
        | ( (((hb_uint16)buf[1]))		& 0xff );
}

hb_uint16 BytesToUI16LE(const char* buf)
{
    return ( (((hb_uint16)buf[1]) << 8 )	& 0xff00 )
        | ( (((hb_uint16)buf[0]))		& 0xff );
}

hb_uint24 BytesToUI24(const char* buf)
{
    return ( (((hb_uint24)buf[0]) << 16)	& 0xff0000 )
        | ( (((hb_uint24)buf[1]) << 8)	& 0xff00 )
        | ( (((hb_uint24)buf[2]))		& 0xff );
}

hb_uint32 BytesToUI32(const char* buf)
{
    return ( (((hb_uint32)buf[0]) << 24)	& 0xff000000 )
        | ( (((hb_uint32)buf[1]) << 16)	& 0xff0000 )
        | ( (((hb_uint32)buf[2]) << 8)	& 0xff00 )
        | ( (((hb_uint32)buf[3]))		& 0xff );
}

hb_uint32 BytesToUI32LE(const char* buf)
{
    return ( (((hb_uint32)buf[3]) << 24)	& 0xff000000 )
        | ( (((hb_uint32)buf[2]) << 16)	& 0xff0000 )
        | ( (((hb_uint32)buf[1]) << 8)	& 0xff00 )
        | ( (((hb_uint32)buf[0]))		& 0xff );
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

hb_uint64 BytesToUI64LE(const char* buf)
{
    return ( (((hb_uint64)buf[7]) << 56)	& 0xff00000000000000 )
        | ( (((hb_uint64)buf[6]) << 48)	& 0xff000000000000 )
        | ( (((hb_uint64)buf[5]) << 40)	& 0xff0000000000 )
        | ( (((hb_uint64)buf[4]) << 32)	& 0xff00000000 )
        | ( (((hb_uint64)buf[3]) << 24)	& 0xff000000 )
        | ( (((hb_uint64)buf[2]) << 16)	& 0xff0000 )
        | ( (((hb_uint64)buf[1]) << 8)	& 0xff00 )
        | ( (((hb_uint64)buf[0]))			& 0xff );
}

double BytesToDouble(const char* buf) 
{
    unsigned char b[8];
    memcpy(b, buf, 8);

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

char* UI08ToBytes(char* buf, hb_uint8 val)
{
    char* pbuf = buf;
    buf[0] = (char)(val) & 0xff;
    pbuf += 1;
    return pbuf;
}

char* UI08ToBytesLE(char* buf, hb_uint8 val)
{
    char* pbuf = buf;
    buf[0] = (char)(val) & 0xff;
    pbuf += 1;
    return pbuf;
}

char* UI16ToBytes(char* buf, hb_uint16 val)
{
    char* pbuf = buf;
    buf[0] = (char)(val >> 8) & 0xff;
    buf[1] = (char)(val) & 0xff;
    pbuf += 2;
    return pbuf;
}

char* UI16ToBytesLE(char* buf, hb_uint16 val)
{
    char* pbuf = buf;
    buf[1] = (char)(val >> 8) & 0xff;
    buf[0] = (char)(val) & 0xff;
    pbuf += 2;
    return pbuf;
}

char* UI24ToBytes(char* buf, hb_uint24 val)
{
    char* pbuf = buf;
    buf[0] = (char)(val >> 16) & 0xff;
    buf[1] = (char)(val >> 8) & 0xff;
    buf[2] = (char)(val) & 0xff;
    pbuf += 3;
    return pbuf;
}

char* UI24ToBytesLE(char* buf, hb_uint24 val)
{
    char* pbuf = buf;
    buf[2] = (char)(val >> 16) & 0xff;
    buf[1] = (char)(val >> 8) & 0xff;
    buf[0] = (char)(val) & 0xff;
    pbuf += 3;
    return pbuf;
}

char* UI32ToBytes(char* buf, hb_uint32 val)
{
    char* pbuf = buf;
    buf[0] = (char)(val >> 24) & 0xff;
    buf[1] = (char)(val >> 16) & 0xff;
    buf[2] = (char)(val >> 8) & 0xff;
    buf[3] = (char)(val) & 0xff;
    pbuf += 4;
    return pbuf;
}

char* UI32ToBytesLE(char* buf, hb_uint32 val)
{
    char* pbuf = buf;
    buf[3] = (char)(val >> 24) & 0xff;
    buf[2] = (char)(val >> 16) & 0xff;
    buf[1] = (char)(val >> 8) & 0xff;
    buf[0] = (char)(val) & 0xff;
    pbuf += 4;
    return pbuf;
}

char* UI64ToBytes(char* buf, hb_uint64 val)
{
    char* pbuf = buf;
    buf[0] = (char)(val >> 56) & 0xff;
    buf[1] = (char)(val >> 48) & 0xff;
    buf[2] = (char)(val >> 40) & 0xff;
    buf[3] = (char)(val >> 32) & 0xff;
    buf[4] = (char)(val >> 24) & 0xff;
    buf[5] = (char)(val >> 16) & 0xff;
    buf[6] = (char)(val >> 8) & 0xff;
    buf[7] = (char)(val) & 0xff;
    pbuf += 8;
    return pbuf;
}

char* UI64ToBytesLE(char* buf, hb_uint64 val)
{
    char* pbuf = buf;
    buf[7] = (char)(val >> 56) & 0xff;
    buf[6] = (char)(val >> 48) & 0xff;
    buf[5] = (char)(val >> 40) & 0xff;
    buf[4] = (char)(val >> 32) & 0xff;
    buf[3] = (char)(val >> 24) & 0xff;
    buf[2] = (char)(val >> 16) & 0xff;
    buf[1] = (char)(val >> 8) & 0xff;
    buf[0] = (char)(val) & 0xff;
    pbuf += 8;
    return pbuf;
}

char* DoubleToBytes(char* buf, double val) 
{
    char* pbuf = buf;
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

    memcpy(buf, b, 8);

    pbuf += 8;
    return pbuf;
}
