#pragma once
#include "HuolunCore/HComponent.h"
#include <unistd.h>
#include <iostream>
using namespace std;
class StdinComponent
    :public HComponent
{
public:
    StdinComponent()
    {
        mHandle = STDIN_FILENO;
    }
    virtual bool Initialize()override
    {
        return true;
    }
    virtual bool Finish()override
    {
        return true;
    }
    virtual bool OnRead(HBuffer *buffer)override
    {
        string strBuf;
        cin>>strBuf;
        buffer->Append(strBuf);
        return true;
    }
    virtual bool OnWrite(const HBuffer *buffer)override
    {
        return false;
    }
};
