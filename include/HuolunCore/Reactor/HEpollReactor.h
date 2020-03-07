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
    virtual void Run()override;
    virtual void Stop()override;
protected:
    virtual bool RegisterReadImpl(HIOChannel *ch)override;
    virtual bool UnregisterReadImpl(HIOChannel *ch)override;
    virtual bool RegisterWriteImpl(HIOChannel *ch)override;
    virtual bool UnregisterWriteImpl(HIOChannel *ch)override;
    bool EpollCtl(HIOChannel *ch,int op ,int events);
private:
    int mEpfd = -1;
    std::map<handle_t,HIOChannel*> mMapOfHandleChannel;
    std::set<HIOChannel*> mSetOfChannel;
};
#endif
