#pragma once
#include "HuolunCore/HPlatform.h"
#include "HReactor.h"
#include "HReactorFactory.h"
class HSelectReactor
    :public HReactor
{
public:
    HSelectReactor();
    ~HSelectReactor();
    virtual void Run()override;
    virtual void Stop()override;
protected:
    virtual bool RegisterReadImpl(HIOChannel *ch)override;
    virtual bool UnregisterReadImpl(HIOChannel *ch)override;
    virtual bool RegisterWriteImpl(HIOChannel *ch)override;
    virtual bool UnregisterWriteImpl(HIOChannel *ch)override;
    bool KqueueModFilter(HIOChannel *ch,int filter,int flag);
};
class HSelectReactorFactory
    :public HReactorFactory
{
public:
    virtual HReactor *CreateReactor()override;
};
