#pragma once
#include "HuolunCore/HPlatform.h"
#ifdef __HUOLUN_IOCP__
#include "HReactor.h"
#include "HReactorFactory.h"

class HIOCPReactor
    :public HReactor
{
public:
    HIOCPReactor();
    ~HIOCPReactor();
    virtual void Run()override;
    virtual void Stop()override;
protected:
    virtual bool RegisterReadImpl(HIOChannel *ch)override;
    virtual bool UnregisterReadImpl(HIOChannel *ch)override;
    virtual bool RegisterWriteImpl(HIOChannel *ch)override;
    virtual bool UnregisterWriteImpl(HIOChannel *ch)override;
};
class HIOCPReactorFactory
    :public HReactorFactory
{
public:
    virtual HReactor *CreateReactor()override;
};
#endif
