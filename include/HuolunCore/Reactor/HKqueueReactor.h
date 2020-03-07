#pragma once
#include "HuolunCore/HPlatform.h"
#ifdef __HUOLUN_PLATFORM_APPLE__
#include "HReactor.h"
#include <map>
#include <set>
class HKqueueReactor
    :public HReactor
{
public:
    HKqueueReactor();
    ~HKqueueReactor();
    virtual bool Initialize()override;
    virtual bool Finish()override;
    virtual bool Install(HIOChannel *ch)override;
    virtual bool Uninstall(handle_t handle)override;
    virtual void Run()override;
    virtual void Stop()override;
protected:
    virtual bool RegisterReadImpl(HIOChannel *ch)override;
    virtual bool UnregisterReadImpl(HIOChannel *ch)override;
    virtual bool RegisterWriteImpl(HIOChannel *ch)override;
    virtual bool UnregisterWriteImpl(HIOChannel *ch)override;
    bool KqueueModFilter(HIOChannel *ch,int filter,int flag);
private:
    int mKqueueFd = -1;
    //TODO : Container choose optimize
    std::map<handle_t,HIOChannel*> mMapOfHandleChannel;
    std::set<HIOChannel*> mSetOfChannel;
};
#endif
