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
    virtual bool Register(HIOChannel *ch,EIOStat flag=EIOStat::In)=0;
    virtual bool Unregister(handle_t handle)=0;
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
