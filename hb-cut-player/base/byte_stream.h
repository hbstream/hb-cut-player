#ifndef _BYTE_STREAM_H_
#define _BYTE_STREAM_H_

#include "base/typedefs.h"

#include <stdlib.h>

hb_uint8 BytesToUI08(const char* buf);

hb_uint8 BytesToUI08LE(const char* buf);

hb_uint16 BytesToUI16(const char* buf);

hb_uint16 BytesToUI16LE(const char* buf);

hb_uint24 BytesToUI24(const char* buf);

hb_uint32 BytesToUI32(const char* buf);

hb_uint32 BytesToUI32LE(const char* buf);

hb_uint64 BytesToUI64(const char* buf);

hb_uint64 BytesToUI64LE(const char* buf);

double BytesToDouble(const char* buf);

char* UI08ToBytes(char* buf, hb_uint8 val);

char* UI08ToBytesLE(char* buf, hb_uint8 val);

char* UI16ToBytes(char* buf, hb_uint16 val);

char* UI16ToBytesLE(char* buf, hb_uint16 val);

char* UI24ToBytes(char* buf, hb_uint24 val);

char* UI24ToBytesLE(char* buf, hb_uint24 val);

char* UI32ToBytes(char* buf, hb_uint32 val);

char* UI32ToBytesLE(char* buf, hb_uint32 val);

char* UI64ToBytes(char* buf, hb_uint64 val);

char* UI64ToBytesLE(char* buf, hb_uint64 val);

char* DoubleToBytes(char* buf, double val);

#endif // _BYTE_STREAM_H_
