/*******************************************************************************
 * buf_unit_helper.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2014-11-16 10:51 - Created (Haibin Du)
 ******************************************************************************/

#include "buf_unit_helper.h"

#include <cstring>

BufUnitHelper::BufUnitHelper(int unitSize)
{
    total_size_ = 0;
    current_pos_ = 0;

    unit_size_ = unitSize;
    unit_buf_ = new char[unit_size_];
}

BufUnitHelper::~BufUnitHelper()
{
    while (false == buf_queue_.empty())
    {
        HelperBuffer* databuf = buf_queue_.front();
        delete databuf;
        buf_queue_.pop_front();
    }
    delete[] unit_buf_;
}

bool BufUnitHelper::ReadBuf()
{
    if (total_size_ < unit_size_)
    {
        return false;
    }

    char* pbuf = unit_buf_;
    int buflen = 0;

    while (true)
    {
        HelperBuffer* tmpbuf = buf_queue_.front();
        if (current_pos_ > 0)
        {
            if (tmpbuf->data_len-current_pos_ >= unit_size_)
            {
                memcpy(pbuf, tmpbuf->data+current_pos_, unit_size_);
                buflen += unit_size_;
                total_size_ -= unit_size_;

                current_pos_ += unit_size_;
                if (current_pos_ >= tmpbuf->data_len)
                {
                    buf_queue_.pop_front();
                    current_pos_ = 0;
                    delete tmpbuf;
                }
                break;
            }
            else
            {
                memcpy(pbuf, tmpbuf->data+current_pos_, tmpbuf->data_len-current_pos_);
                pbuf += (tmpbuf->data_len-current_pos_);
                buflen += (tmpbuf->data_len-current_pos_);
                total_size_ -= (tmpbuf->data_len-current_pos_);

                current_pos_ = 0;
                buf_queue_.pop_front();
                delete tmpbuf;
            }
        }
        else
        {
            if (buflen + tmpbuf->data_len > unit_size_)
            {
                int needsize = unit_size_ - buflen;
                memcpy(pbuf, tmpbuf->data, needsize);
                pbuf += needsize;
                buflen += needsize;
                total_size_ -= needsize;

                current_pos_ = needsize;
                break;
            }
            else if (buflen + tmpbuf->data_len == unit_size_)
            {
                memcpy(pbuf, tmpbuf->data, tmpbuf->data_len);
                pbuf += tmpbuf->data_len;
                buflen += tmpbuf->data_len;
                total_size_ -= tmpbuf->data_len;

                current_pos_ = 0;
                buf_queue_.pop_front();
                delete tmpbuf;
                break;
            }
            else
            {
                memcpy(pbuf, tmpbuf->data, tmpbuf->data_len);
                pbuf += tmpbuf->data_len;
                buflen += tmpbuf->data_len;
                total_size_ -= tmpbuf->data_len;

                buf_queue_.pop_front();
                delete tmpbuf;
            }
        }
    }

    return true;
}

void BufUnitHelper::PushBuf(char* buf, int bufLen)
{
    buf_queue_.push_back(new HelperBuffer(buf, bufLen));
    total_size_ += bufLen;
}
