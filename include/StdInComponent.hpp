#pragma once
#include "HComponent.h"
#include "unistd.h"
#include <iostream>
using namespace std;
class StdInComponent
    :public  HComponent
{
public:
    virtual bool Initialize()override
    {
        return true;
    }
    virtual bool Finish()override
    {
        return true;
    }
    virtual handle_t GetHandle()override
    {
        return STDIN_FILENO;
    }
    virtual bool OnRead(std::string &readContent)override
    {
        cin>>readContent;
        return true;
    }
    virtual bool OnWrite(const std::string &writeContent)override
    {
        return false;
    }
    virtual void OnError(const std::string &error)override
    {
    }
};
