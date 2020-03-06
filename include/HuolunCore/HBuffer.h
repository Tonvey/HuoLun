#pragma once
#include "HObject.h"
#include <string>
typedef char buffer_element_t;
class HBuffer
    :public HObject
{
public:
    HBuffer();
    HBuffer(size_t size);
    HBuffer(const std::string &strBytes);
    HBuffer(const char *buf, int len);
    HBuffer(const HBuffer &other);
    HBuffer(HBuffer &&other);
    HBuffer &operator =(const HBuffer &other);
    HBuffer &operator =(HBuffer &&oter);
    ~HBuffer();
    void Append(const HBuffer &content);
    void Append(const char *buf,int len);
    void Append(const std::string &strBytes);
    void Resize(size_t newSize);
    void Clear();
    buffer_element_t *GetData()const{return mBuf;}
    size_t GetSize()const{return mSize;};
    size_t GetReservedSize()const{return mReservedSize;};
    HBuffer *Clone();
    static HBuffer *Clone(const HBuffer *sample);
    void CopyFrom(const HBuffer &other);
    void MoveFrom(HBuffer &other);
private:
    void Reallocate(size_t size);
    buffer_element_t *mBuf=nullptr;
    size_t mSize=0;
    size_t mReservedSize=0;
};
