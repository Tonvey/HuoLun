#pragma once
#include "HRef.h"
#include "HTypes.h"
class HObject :
    public HRef
{
public:
    HObject(){}
    virtual ~HObject(){}
    template<class T>
    static T *Create()
    {
        auto ret = new T();
        ret->Retain();
        return ret;
    }
};
