#pragma once
#include "HRef.h"
#include "HTypes.h"
#include <type_traits>
#include <cassert>
class HObject :
    public HRef
{
public:
    HObject(){}
    HObject(const HObject &)=delete;
    HObject(HObject &&)=delete;
    virtual ~HObject(){}
    template<class T,class ... Args>
    static T *Create(Args... args)
    {
        assert((std::is_base_of<HObject,T>::value));
        auto ret = new T(args...);
        ret->Retain();
        return ret;
    }
private:
    void *operator new(size_t size)
    {
        return ::operator new(size);
    }
};
template<class T,class ... Args>
T *CreateObject(Args... args)
{
    return HObject::Create<T>(args...);
}
