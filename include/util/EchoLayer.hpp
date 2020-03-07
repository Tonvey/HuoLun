#pragma once
#include "HuolunCore/HComponent.h"
#include "unistd.h"
#include <iostream>
using namespace std;
class EchoLayer
    :public HLayer
{
public:
    void SetDstLayer(HLayer *dst){this->dst=dst;}
    virtual HMessage *HandleMessageForward(HMessage *msg)
    {
        if(dst)
        {
            dst->Handle(HMessageHandler::HandleDirection::Backward,msg);
        }
        return nullptr;
    }
    virtual HMessage *HandleMessageBackward(HMessage *msg)
    {
        return nullptr;
    }
private:
    HLayer *dst=nullptr;
};