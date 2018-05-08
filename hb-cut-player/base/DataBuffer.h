#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_

#include "base.h"

#include <deque>
#include <set>
#include <vector>

#include "Lock.h"

namespace base{

class DataBuffer
{
public:
    static DataBuffer* CopyMake(char* buf, unsigned int bufLen)
    {
        char* buf_clone = new char[bufLen];
        memcpy(buf_clone, buf, bufLen);
        DataBuffer* data_buf = new DataBuffer(buf_clone, bufLen, true);
        return data_buf;
    }

public:
    DataBuffer(char* buf, unsigned int bufLen, bool isNeedFree = false)
    {
        buf_ = buf;
        buf_len_ = bufLen;
        is_need_free_ = isNeedFree;
    }

    DataBuffer(const DataBuffer& other)
    {
        buf_ = other.buf_;
        buf_len_ = other.buf_len_;
        is_need_free_ = other.is_need_free_;
    }

    ~DataBuffer()
    {
        if (is_need_free_)
        {
            delete[] buf_;
        }
    }

    char* Buf() { return buf_; }

    unsigned int BufLen() { return buf_len_; }

    DataBuffer* Clone()
    {
        char* buf_clone = new char[buf_len_];
        memcpy(buf_clone, buf_, buf_len_);
        DataBuffer* data_buf = new DataBuffer(buf_clone, buf_len_, true);
        return data_buf;
    }

private:
    char* buf_;
    unsigned int buf_len_;
    bool is_need_free_;
};

class DataBufferCache
{
public:
    DataBufferCache();

    ~DataBufferCache();

    DataBuffer* MallocData(int srcSize);

    DataBuffer* MallocData(const char* srcData, int srcSize);

    void FreeData(DataBuffer* dataBuffer);

    void DropOneCache();

    void Clear();

    int CacheCount();

private:
    std::deque<DataBuffer*> cache_list_;
    std::set<DataBuffer*> using_cache_list_;

    base::Lock list_mtx_;
};

class MemBufCache
{
public:
    explicit MemBufCache(int unitSize);

    ~MemBufCache();

    char* MallocBuf();

    void FreeBuf(char* memBuf);

    void Clear();

    int CacheCount();

    int UnitSize() const { return unit_size_; }

private:
    std::deque<char*> cache_list_;
    std::set<char*> using_cache_list_;
    int unit_size_;

    base::Lock list_mtx_;
};

class ResizeBuf
{
public:
    explicit ResizeBuf(int capacity)
    {
        buf_capacity_ = capacity;
        buf_ = new std::vector<char>(buf_capacity_);
        data_size_ = 0;
    }

    ~ResizeBuf()
    {
        delete buf_;
    }

    char* DataBuf() { return &(*buf_)[0]; }

    int DataSize() { return data_size_; }

    void SetSize(int dataSize) { data_size_ = dataSize; }

    void Clear() { data_size_ = 0; }

    void MakeSureCapacity(int needCapacity)
    {
        if (needCapacity > buf_capacity_)
        {
            delete buf_;
            buf_capacity_ = needCapacity;
            buf_ = new std::vector<char>(buf_capacity_);
            data_size_ = 0;
        }
    }

private:
    std::vector<char>* buf_;
    int buf_capacity_;
    int data_size_;
};

class ResizeBufCache
{
public:
    explicit ResizeBufCache();

    ~ResizeBufCache();

    ResizeBuf* MallocBuf(int needSize);

    void FreeBuf(ResizeBuf* resizeBuf);

    void Clear();

    int CacheCount();

private:
    std::deque<ResizeBuf*> cache_list_;
    std::set<ResizeBuf*> using_cache_list_;
};

//////////////////////////////////////////////////////////////////////////

class RefCountDataBuffer : public DataBuffer
{
public:
    RefCountDataBuffer(char* buf, unsigned int bufLen, bool isNeedFree = false)
        : DataBuffer(buf, bufLen, isNeedFree)
    {
        ref_count_ = 1;
    }

    RefCountDataBuffer(DataBuffer* dataBuffer)
        : DataBuffer(*dataBuffer)
    {
        ref_count_ = 1;
    }

    virtual ~RefCountDataBuffer()
    {
    }

    int RefCount() const { return ref_count_; }

    void DecRef() { ref_count_--; }

    void ResetRef(int setCount) { ref_count_ = setCount; }

private:
    DISALLOW_COPY_AND_ASSIGN(RefCountDataBuffer);

private:
    int ref_count_;
};

class RefCountDataBufferCache
{
public:
    RefCountDataBufferCache();

    ~RefCountDataBufferCache();

    RefCountDataBuffer* MallocData(const char* srcData, int srcSize);

    void FreeData(RefCountDataBuffer* dataBuffer);

    void Clear();

    int CacheCount();

private:
    std::deque<RefCountDataBuffer*> cache_list_;
    std::set<RefCountDataBuffer*> using_cache_list_;

    base::Lock list_mtx_;
};

} // namespace Base

#endif // _DATA_BUFFER_H_
