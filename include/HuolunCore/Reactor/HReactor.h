#pragma once
#include "HuolunCore/HObject.h"
#include "HuolunCore/HTypes.h"
class HIOChannel;
class HReactor
    :public HObject
{
public:
    HReactor(){}
    virtual ~HReactor(){}
    virtual bool Initialize()=0;
    virtual bool Finish()=0;
    virtual bool Install(HIOChannel *ch)=0;
    virtual bool Uninstall(handle_t handle)=0;
    virtual bool RegisterRead(HIOChannel *ch)=0;
    virtual bool RegisterWrite(HIOChannel *ch)=0;
    virtual bool UnregisterRead(HIOChannel *ch)=0;
    virtual bool UnregisterWrite(HIOChannel *ch)=0;
    virtual void Run()=0;
    virtual void Stop()=0;
protected:
    enum RunningFlag
    {
        Created,
        Initialized,
        Running,
    } mRunningFlag;
};
