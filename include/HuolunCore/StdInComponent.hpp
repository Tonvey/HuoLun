#pragma once
#include "HComponent.h"
#include "unistd.h"
#include <iostream>
using namespace std;
class StdInComponent
    :public HComponent
{
public:
    virtual bool Initialize()override
    {
        mHandle = STDIN_FILENO;
        return true;
    }
    virtual bool Finish()override
    {
        return true;
    }
    virtual bool OnRead(HBuffer *buffer)override
    {
        Handle(HMessageHandler::Forward,nullptr);
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
