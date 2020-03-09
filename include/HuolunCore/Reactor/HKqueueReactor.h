#pragma once
#include "HuolunCore/HPlatform.h"
#ifdef __HUOLUN_KQUEUE__
#include "HReactor.h"
#include "HReactorFactory.h"
class HKqueueReactor
    :public HReactor
{
public:
    HKqueueReactor();
    ~HKqueueReactor();
    virtual bool Initialize()override;
    virtual void Finish()override;
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
};
class HKqueueReactorFactory
    :public HReactorFactory
{
public:
    virtual HReactor *CreateReactor()override;
};
#endif
