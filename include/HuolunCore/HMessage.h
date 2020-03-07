#pragma once
#include "HBuffer.h"
class HMessage
    :public HObject
{
};

class HBytesMessage
    :public HMessage
{
public:
    HBytesMessage()
    {
        mBytes = Create<HBuffer>();
    }
    ~HBytesMessage()
    {
        if(mBytes)
        {
            mBytes->Release();
        }
    }
    HBuffer *GetBytes()const {return mBytes;}
private:
    HBuffer *mBytes;
};
