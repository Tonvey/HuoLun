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
    virtual void Run()=0;
    virtual void Stop()=0;
protected:
    virtual bool RegisterReadImpl(HIOChannel *ch)=0;
    virtual bool RegisterWriteImpl(HIOChannel *ch)=0;
    virtual bool UnregisterReadImpl(HIOChannel *ch)=0;
    virtual bool UnregisterWriteImpl(HIOChannel *ch)=0;
public:
    bool RegisterRead(HIOChannel *ch);
    bool RegisterWrite(HIOChannel *ch);
    bool UnregisterRead(HIOChannel *ch);
    bool UnregisterWrite(HIOChannel *ch);
protected:
    enum RunningFlag
    {
        Created,
        Initialized,
        Running,
    } mRunningFlag;
};
