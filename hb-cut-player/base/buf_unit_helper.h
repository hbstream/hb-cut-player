/*******************************************************************************
 * buf_unit_helper.h
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * 输入不定长度的buf数据，输出定长buf数据
 *
 * -----------------------------------------------------------------------------
 * 2014-11-16 10:50 - Created (Haibin Du)
 ******************************************************************************/

#ifndef _HDEV_BUF_UNIT_HELPER_H_
#define _HDEV_BUF_UNIT_HELPER_H_

#include <cstring>
#include <deque>

struct HelperBuffer
{
    char* data;
    int data_len;

    HelperBuffer(char* buf, int bufLen)
    {
        data = new char[bufLen];
        memcpy(data, buf, bufLen);
        data_len = bufLen;
    }

    ~HelperBuffer()
    {
        delete[] data;
    }
};

class BufUnitHelper
{
public:
    BufUnitHelper(int unitSize);

    ~BufUnitHelper();

    bool ReadBuf();

    void PushBuf(char* buf, int bufLen);

    int TotalSize() { return total_size_; }

    char* UnitBuf() { return unit_buf_; }
    int UnitSize() { return unit_size_; }

private:
    std::deque<HelperBuffer*> buf_queue_;
    int total_size_;
    int current_pos_;
    int unit_size_;
    char* unit_buf_;
};

#endif // _HBASE_BUF_UNIT_HELPER_H_
