#pragma once
#include "HuolunCore/HPlatform.h"
#ifdef __HUOLUN_PLATFORM_LINUX__
#include "HReactor.h"
#include "HReactorFactory.h"
class HEpollReactor
    :public HReactor
{
public:
    HEpollReactor();
    ~HEpollReactor();
    virtual bool Initialize()override;
    virtual void Finish()override;
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
};
class HEpollReactorFactory
    :public HReactorFactory
{
public:
    virtual HReactor *CreateReactor()override;
};
#endif
