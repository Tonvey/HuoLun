#pragma once
#include "HuolunCore/HComponent.h"
#include <iostream>
using namespace std;
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
class StdinComponent
    :public HComponent
{
public:
    StdinComponent()
    {
    }
    virtual bool Initialize()override
    {
        mHandle = STDIN_FILENO;
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
