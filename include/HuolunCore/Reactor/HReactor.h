#pragma once
#include "HuolunCore/HObject.h"
#include "HuolunCore/HTypes.h"
#include <map>
#include <set>
class HIOChannel;
class HReactor
    :public HObject
{
public:
    HReactor();
    virtual ~HReactor();
    virtual bool Initialize();
    virtual void Finish();
    virtual void Run()=0;
    virtual void Stop()=0;
protected:
    virtual bool RegisterReadImpl(HIOChannel *ch)=0;
    virtual bool RegisterWriteImpl(HIOChannel *ch)=0;
    virtual bool UnregisterReadImpl(HIOChannel *ch)=0;
    virtual bool UnregisterWriteImpl(HIOChannel *ch)=0;
public:
    bool Install(HIOChannel *ch);
    bool Uninstall(handle_t handle);
    bool Uninstall(HIOChannel *ch);
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
    } mRunningFlag=Created;
private:
    std::map<handle_t,HIOChannel*> mMapOfHandleChannel;
    std::set<HIOChannel*> mSetOfChannel;
};
