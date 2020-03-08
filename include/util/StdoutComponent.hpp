#pragma once
#include "HuolunCore/HComponent.h"
#include <iostream>
using namespace std;
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
class StdoutComponent
    :public HComponent
{
public:
    StdoutComponent()
    {
    }
    virtual bool Initialize()override
    {
        mHandle = STDOUT_FILENO;
        return true;
    }
    virtual bool Finish()override
    {
        return true;
    }
    virtual bool OnRead(HBuffer *buffer)override
    {
        return false;
    }
    virtual bool OnWrite(const HBuffer *buffer)override
    {
        std::string str(buffer->GetData(),buffer->GetSize());
        cout<<str<<endl;
        return true;
    }
};
