#pragma once
#include "HuolunCore/HComponent.h"
#include <unistd.h>
#include <iostream>
using namespace std;
class StdoutComponent
    :public HComponent
{
public:
    StdoutComponent()
    {
        mHandle = STDOUT_FILENO;
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
        return false;
    }
    virtual bool OnWrite(const HBuffer *buffer)override
    {
        std::string str(buffer->GetData(),buffer->GetSize());
        cout<<str<<endl;
        return true;
    }
};
