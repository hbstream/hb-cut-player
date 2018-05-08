/*******************************************************************************
 * DataBuffer.cpp
 * Copyright: (c) 2014 Haibin Du(haibindev.cnblogs.com). All rights reserved.
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 2015-12-2 9:54 - Created (Haibin Du)
 ******************************************************************************/

#include "DataBuffer.h"

namespace base{

//////////////////////////////////////////////////////////////////////////

DataBufferCache::DataBufferCache()
{

}

DataBufferCache::~DataBufferCache()
{
    Clear();

}

DataBuffer* DataBufferCache::MallocData(int srcSize)
{
    DataBuffer* new_data = NULL;

    if (cache_list_.empty())
    {
        char* databuf = new char[srcSize];
        new_data = new DataBuffer(databuf, srcSize, true);
    }
    else
    {
        new_data = cache_list_.front();

        if (new_data->BufLen() != srcSize)
        {
            return NULL;
        }

        cache_list_.pop_front();
    }

    using_cache_list_.insert(new_data);

    return new_data;
}

DataBuffer* DataBufferCache::MallocData(const char* srcData,
    int srcSize)
{
    DataBuffer* new_data = NULL;

    if (cache_list_.empty())
    {
        new_data = DataBuffer((char*)srcData, srcSize).Clone();
    }
    else
    {
        new_data = cache_list_.front();

        if (new_data->BufLen() != srcSize)
        {
            return NULL;
        }

        memcpy(new_data->Buf(), srcData, srcSize);
        cache_list_.pop_front();
    }

    using_cache_list_.insert(new_data);

    return new_data;
}

void DataBufferCache::FreeData(DataBuffer* dataBuffer)
{
    using_cache_list_.erase(dataBuffer);

    cache_list_.push_back(dataBuffer);
}

void DataBufferCache::DropOneCache()
{
    if (cache_list_.empty()) return;

    DataBuffer* data = cache_list_.front();
    cache_list_.pop_front();

    delete data;
}

void DataBufferCache::Clear()
{
    for (auto it = cache_list_.begin(); it != cache_list_.end(); ++it)
    {
        DataBuffer* data = *it;

        delete data;
    }
    cache_list_.clear();

    for (auto it = using_cache_list_.begin(); it != using_cache_list_.end(); ++it)
    {
        DataBuffer* data = *it;

        delete data;
    }
    using_cache_list_.clear();
}

int DataBufferCache::CacheCount()
{
    return cache_list_.size() + using_cache_list_.size();
}


//////////////////////////////////////////////////////////////////////////

MemBufCache::MemBufCache(int unitSize)
{
    unit_size_ = unitSize;
}

MemBufCache::~MemBufCache()
{
    Clear();
}

char* MemBufCache::MallocBuf()
{
    char* new_mem = NULL;

    if (cache_list_.empty())
    {
        new_mem = new char[unit_size_];
    }
    else
    {
        new_mem = cache_list_.front();

        cache_list_.pop_front();
    }

    using_cache_list_.insert(new_mem);

    return new_mem;
}

void MemBufCache::FreeBuf(char* memBuf)
{
    using_cache_list_.erase(memBuf);

    cache_list_.push_back(memBuf);
}

void MemBufCache::Clear()
{
    for (auto it = cache_list_.begin(); it != cache_list_.end(); ++it)
    {
        char* data = *it;

        delete[] data;
    }
    cache_list_.clear();

    for (auto it = using_cache_list_.begin(); it != using_cache_list_.end(); ++it)
    {
        char* data = *it;

        delete[] data;
    }
    using_cache_list_.clear();
}

int MemBufCache::CacheCount()
{
    return cache_list_.size() + using_cache_list_.size();
}

//////////////////////////////////////////////////////////////////////////


ResizeBufCache::ResizeBufCache()
{

}

ResizeBufCache::~ResizeBufCache()
{
    Clear();
}

ResizeBuf* ResizeBufCache::MallocBuf(int needSize)
{
    ResizeBuf* new_buf = NULL;

    if (cache_list_.empty())
    {
        new_buf = new ResizeBuf(needSize);
    }
    else
    {
        new_buf = cache_list_.front();
        cache_list_.pop_front();

        new_buf->MakeSureCapacity(needSize);
    }

    using_cache_list_.insert(new_buf);

    return new_buf;
}

void ResizeBufCache::FreeBuf(ResizeBuf* resizeBuf)
{
    resizeBuf->Clear();

    using_cache_list_.erase(resizeBuf);

    cache_list_.push_back(resizeBuf);
}

void ResizeBufCache::Clear()
{
    for (auto it = cache_list_.begin(); it != cache_list_.end(); ++it)
    {
        ResizeBuf* buf = *it;

        delete buf;
    }
    cache_list_.clear();

    for (auto it = using_cache_list_.begin(); it != using_cache_list_.end(); ++it)
    {
        ResizeBuf* buf = *it;

        delete buf;
    }
    using_cache_list_.clear();
}

int ResizeBufCache::CacheCount()
{
    return cache_list_.size() + using_cache_list_.size();
}

//////////////////////////////////////////////////////////////////////////

RefCountDataBufferCache::RefCountDataBufferCache()
{

}

RefCountDataBufferCache::~RefCountDataBufferCache()
{
    Clear();
}

RefCountDataBuffer* RefCountDataBufferCache::MallocData(const char* srcData,
    int srcSize)
{
    RefCountDataBuffer* new_data = NULL;

    if (cache_list_.empty())
    {
        new_data = new RefCountDataBuffer(DataBuffer((char*)srcData, srcSize).Clone());
    }
    else
    {
        new_data = cache_list_.front();

        if (new_data->BufLen() != srcSize)
        {
            return NULL;
        }

        memcpy(new_data->Buf(), srcData, srcSize);

        cache_list_.pop_front();
    }

    using_cache_list_.insert(new_data);

    return new_data;
}

void RefCountDataBufferCache::FreeData(RefCountDataBuffer* dataBuffer)
{
    using_cache_list_.erase(dataBuffer);

    cache_list_.push_back(dataBuffer);
}

void RefCountDataBufferCache::Clear()
{
    for (auto it = cache_list_.begin(); it != cache_list_.end(); ++it)
    {
        RefCountDataBuffer* data = *it;

        delete data;
    }
    cache_list_.clear();

    for (auto it = using_cache_list_.begin(); it != using_cache_list_.end(); ++it)
    {
        RefCountDataBuffer* data = *it;

        delete data;
    }
    using_cache_list_.clear();
}

int RefCountDataBufferCache::CacheCount()
{
    return cache_list_.size() + using_cache_list_.size();
}


} // namespace Base
