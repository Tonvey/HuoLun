#include "HuolunCore/HRef.h"
#include <cassert>
HRef::HRef()
    :mRefCount(0)
{
}
HRef::~HRef()
{
    assert(mRefCount==0);
}
void HRef::Retain()
{
    assert(mRefCount>=0);
    ++mRefCount;
}
void HRef::Release()
{
    assert(mRefCount>0);
    --mRefCount;
    if(mRefCount<=0)
    {
        delete this;
    }
}
