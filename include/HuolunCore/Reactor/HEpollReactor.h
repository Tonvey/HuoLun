#pragma once
#include "HuolunCore/HPlatform.h"
#ifdef __HUOLUN_PLATFORM_LINUX__
#include "HReactor.h"
#include <map>
#include <set>
class HEpollReactor
    :public HReactor
{
public:
    HEpollReactor();
    ~HEpollReactor();
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
    bool EpollCtl(HIOChannel *ch,int op ,int events);
private:
    int mEpfd = -1;
    std::map<handle_t,HIOChannel*> mMapOfHandleChannel;
    std::set<HIOChannel*> mSetOfChannel;
};
#endif
