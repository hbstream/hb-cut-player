#ifndef _BIT_WRITTER_H_
#define _BIT_WRITTER_H_

#include "base/byte_stream.h"

typedef struct PutBitContext
{
    unsigned int bit_buf;
    int bit_left;
    char *buf, *buf_ptr, *buf_end;
    int size_in_bits;
} PutBitContext;

static inline void init_put_bits(PutBitContext *s, char *buffer, int buffer_size)
{
    if(buffer_size < 0) {
        buffer_size = 0;
        buffer = 0;
    }

    s->size_in_bits= 8*buffer_size;
    s->buf = buffer;
    s->buf_end = s->buf + buffer_size;
    s->buf_ptr = s->buf;
    s->bit_left=32;
    s->bit_buf=0;
}

static inline void flush_put_bits(PutBitContext *s)
{
    s->bit_buf<<= s->bit_left;
    while (s->bit_left < 32) {
        *s->buf_ptr++=s->bit_buf >> 24;
        s->bit_buf<<=8;
        s->bit_left+=8;
    }
    s->bit_left=32;
    s->bit_buf=0;
}


static inline void put_bits(PutBitContext *s, int n, unsigned int value)
{
    unsigned int bit_buf;
    int bit_left;

    bit_buf = s->bit_buf;
    bit_left = s->bit_left;

    if (n < bit_left) {
        bit_buf = (bit_buf<<n) | value;
        bit_left-=n;
    } else {
        bit_buf<<=bit_left;
        bit_buf |= value >> (n - bit_left);
        UI32ToBytes(s->buf_ptr, bit_buf);
        //AV_WB32(s->buf_ptr, bit_buf);
        //printf("bitbuf = %08x\n", bit_buf);
        s->buf_ptr+=4;
        bit_left+=32 - n;
        bit_buf = value;
    }

    s->bit_buf = bit_buf;
    s->bit_left = bit_left;
}

#endif // _BIT_WRITTER_H_
