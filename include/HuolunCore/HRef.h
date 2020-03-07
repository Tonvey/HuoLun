#pragma once
#include <cstdlib>
class HRef
{
public:
    HRef();
    virtual ~HRef();
    void Retain();
    void Release();
    int mRefCount;
};
