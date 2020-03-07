#pragma once
#ifdef __HUOLUN_PLATFORM_APPLE__
#include "HReactor.h"
class HKqueueReactor
    :public HReactor
{
public:
    HKqueueReactor();
    ~HKqueueReactor();
    virtual bool Initialize()=0;
    virtual bool Finish()=0;
    virtual bool Register(HIOChannel *ch,EIOStat flag=EIOStat::In)=0;
    virtual bool Unregister(handle_t handle)=0;
    virtual void Run()=0;
    virtual void Stop()=0;
private:
    int mKqueueFd = -1;
    //TODO : Container choose optimize
    std::map<handle_t,HIOChannel*> mMapOfHandleChannel;
};
#endif
