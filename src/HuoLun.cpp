#include "HuoLun.h"
#include "HComponent.h"
#include <unistd.h>
#include <sys/event.h>
Huolun::Huolun()
{
}
virtual ~Huolun::Huolun()
{
    for (auto itr : m_ChannelList)
    {
        itr->Fini();
        delete itr;
    }
    if (mCoreHandle >= 0)
    {
        close(m_kernel_handle);
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
    if(this->mMapOfHandle.find(h)!=mMapOfHandle.end())
    {
        // has add
        return true;
    }
    if(!comp->Initialize())
    {
        return false;
    }
    mMapofHandle.insert(make_pair(h,comp));
    com->retain();
    return true;
}
bool Huolun::RemoveComponent(handle_t handle)
{
    auto it = this->mMapOfHandle.find(handle);
    if(it==mMapOfHandle.end())
    {
        // has add
        return true;
    }
    mMapofHandle.erase(it);
    it->release();
    return false;
}
