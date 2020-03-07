#pragma once
#include "HRef.h"
#include "HTypes.h"
class HObject :
    public HRef
{
protected:
    HObject(){}
    HObject(const HObject &)=delete;
    HObject(HObject &&)=delete;
public:
    virtual ~HObject(){}
    template<class T>
    static T *Create()
    {
        auto ret = new T();
        ret->Retain();
        return ret;
    }
};
