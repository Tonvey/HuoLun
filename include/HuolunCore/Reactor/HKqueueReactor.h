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
    virtual bool RegisterRead(HIOChannel *ch)override;
    virtual bool UnregisterRead(HIOChannel *ch)override;
    virtual bool RegisterWrite(HIOChannel *ch)override;
    virtual bool UnregisterWrite(HIOChannel *ch)override;
    virtual void Run()override;
    virtual void Stop()override;
    bool KqueueModFilter(HIOChannel *ch,int filter,int flag);
private:
    int mKqueueFd = -1;
    //TODO : Container choose optimize
    std::map<handle_t,HIOChannel*> mMapOfHandleChannel;
    std::set<HIOChannel*> mSetOfChannel;
};
#endif
