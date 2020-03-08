#pragma once
#include "HObject.h"
class HBuffer;
class HMessage
    :public HObject
{
};

class HBytesMessage
    :public HMessage
{
public:
    HBytesMessage();
    ~HBytesMessage();
    HBuffer *GetBytes()const {return mBytes;}
private:
    HBuffer *mBytes;
};
