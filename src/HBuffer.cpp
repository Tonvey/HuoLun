#include "HuolunCore/HBuffer.h"
#include <cstring>
#include <cassert>
HBuffer::HBuffer()
{
}
HBuffer::HBuffer(size_t size)
{
    assert(size>=0);
    Reallocate(size);
    mSize = 0;
}
HBuffer::HBuffer(const std::string &strBytes)
{
    Reallocate(strBytes.size());
    strBytes.copy(mBuf,strBytes.size());
    mSize = strBytes.size();
}
HBuffer::HBuffer(const char *buf, int len)
{
    assert(buf!=nullptr&&len>=0);
    mBuf = new buffer_element_t[len];
    memcpy(mBuf,buf,len);
    mSize = len;
}
HBuffer::HBuffer(const HBuffer &other)
{
    assert(&other!=this);
    CopyFrom(other);
}
HBuffer::HBuffer(HBuffer &&other)
{
    assert(&other!=this);
    MoveFrom(other);
}
HBuffer::~HBuffer()
{
    Clear();
}
void HBuffer::Resize(size_t newSize)
{
    this->Reallocate(newSize);
}
void HBuffer::Reallocate(size_t newSize)
{
    auto buf = new buffer_element_t[newSize];
    mReservedSize = newSize;
    if(mBuf!=nullptr)
    {
        mSize = std::min(newSize,mSize);
        memcpy(buf,mBuf,mSize);
        delete [] mBuf;
    }
    mBuf = buf;
}
void HBuffer::Clear()
{
    if(mBuf!=nullptr)
    {
        delete [] mBuf;
    }
    mBuf = nullptr;
    mSize = mReservedSize = 0;
}
void HBuffer::Append(const HBuffer &content)
{
    assert(&content!=this);
    if(mReservedSize-mSize<content.GetSize())
    {
        Reallocate(mSize+content.GetSize());
    }
    memcpy(mBuf+mSize,content.mBuf,content.GetSize());
    mSize += content.GetSize();
}
void HBuffer::Append(const char *buf,int len)
{
    assert(buf!=mBuf);
    if(mReservedSize-mSize<len)
    {
        Reallocate(mSize+len);
    }
    memcpy(mBuf+mSize,buf,len);
    mSize += len;
}
void HBuffer::Append(const std::string &strBytes)
{
    if(mReservedSize-mSize<strBytes.size())
    {
        Reallocate(mSize+strBytes.size());
    }
    strBytes.copy(mBuf+mSize,strBytes.size());
    mSize += strBytes.size();
}
HBuffer *HBuffer::Clone()
{
    return Clone(this);
}
HBuffer *HBuffer::Clone(const HBuffer *sample)
{
    auto ret = Create<HBuffer>();
    ret->Append(*sample);
    return ret;
}
HBuffer &HBuffer::operator =(const HBuffer &other)
{
    CopyFrom(other);
    return *this;
}
HBuffer &HBuffer::operator =(HBuffer &&other)
{
    MoveFrom(other);
    return *this;
}
void HBuffer::CopyFrom(const HBuffer &other)
{
    Clear();
    Reallocate(other.GetSize());
    memcpy(mBuf,other.mBuf,other.GetSize());
    mSize = other.GetSize();
}
void HBuffer::MoveFrom(HBuffer &other)
{
    Clear();
    this->mBuf = other.mBuf;
    this->mSize = other.mSize;
    this->mReservedSize = other.mReservedSize;
    other.mBuf = nullptr;
    other.mSize = other.mReservedSize = 0;
}
