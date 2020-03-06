#pragma once
#include "HObject.h"
#include "HMessage.h"
class HLayer:public HObject
{
public:
    HLayer();
    ~HLayer();
    virtual HMessage *HandleMsg(HMessage *msg)=0;
};
