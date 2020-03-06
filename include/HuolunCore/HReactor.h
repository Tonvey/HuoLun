#pragma once
#include "HObject.h"
#include "HTypes.h"
class HReactor
    :public HObject
{
public:
    HReactor(){}
    virtual ~HReactor(){}
    virtual bool Initialize()=0;
    virtual bool Finish()=0;
};
