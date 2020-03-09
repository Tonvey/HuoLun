#pragma once
#include "HuolunCore/HPlatform.h"
#ifdef __HUOLUN_POLL__
#include "HReactor.h"
#include "HReactorFactory.h"
class HPollReactor
    :public HReactor
{
public:
    HPollReactor();
    ~HPollReactor();
    virtual void Run()override;
    virtual void Stop()override;
protected:
    virtual bool RegisterReadImpl(HIOChannel *ch)override;
    virtual bool UnregisterReadImpl(HIOChannel *ch)override;
    virtual bool RegisterWriteImpl(HIOChannel *ch)override;
    virtual bool UnregisterWriteImpl(HIOChannel *ch)override;
};
class HPollReactorFactory
    :public HReactorFactory
{
public:
    virtual HReactor *CreateReactor()override;
};
#endif
