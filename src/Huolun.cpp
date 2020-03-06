#include "Huolun.h"
#include "HComponent.h"
#include <unistd.h>
#include <sys/event.h>
Huolun::Huolun()
{
}
Huolun::~Huolun()
{
    for (auto itr : mMapOfHandleComponent)
    {
        auto hcomp = itr.second;
        hcomp->Finish();
    }
    mMapOfHandleComponent.clear();
    if (mCoreHandle >= 0)
    {
        close(mCoreHandle);
    }
}
bool Huolun::Initialize()
{
    mCoreHandle = kqueue();
    return mCoreHandle>=0;
}
bool Huolun::Finish()
{
    if(mCoreHandle>=0)
    {
        close(mCoreHandle);
    }
}
bool Huolun::AddComponent(HComponent *comp)
{
    handle_t h = comp->GetHandle();
    if(this->mMapOfHandleComponent.find(h)!=mMapOfHandleComponent.end())
    {
        // has add
        return true;
    }
    if(!comp->Initialize())
    {
        return false;
    }
    mMapOfHandleComponent.insert(std::make_pair(h,comp));
    comp->Retain();
    return true;
}
bool Huolun::RemoveComponent(handle_t handle)
{
    auto it = this->mMapOfHandleComponent.find(handle);
    if(it==mMapOfHandleComponent.end())
    {
        // has add
        return true;
    }
    it->second->Release();
    mMapOfHandleComponent.erase(it);
    return true;
}
