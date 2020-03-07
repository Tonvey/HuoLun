#pragma once
#include "HMessageHandler.h"
class HLayer
    :virtual public HMessageHandler
{
public:
    HLayer(){};
    ~HLayer(){};
    virtual HMessage *HandleMessageForward(HMessage *msg)=0;
    virtual HMessage *HandleMessageBackward(HMessage *msg)=0;
};
