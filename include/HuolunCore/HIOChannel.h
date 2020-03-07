#pragma once
#include "HTypes.h"
#include "HBuffer.h"
class Huolun;
class HReactor;
class HIOChannel
    :virtual public HObject
{
public:
    friend HReactor;
    //Initialize object and create the fd/handle here
    virtual bool Initialize()=0;
    //Call when unregister from reactor core , destroy fd/handle here
    virtual bool Finish()=0;
    virtual void TriggerRead()=0;
    virtual void TriggerWrite()=0;
    virtual handle_t GetHandle()const{return mHandle;};
    virtual bool IsNeedClosed(){return mNeedClosed;};
    void SetClosed(bool val){mNeedClosed = val;};
    void SetReactor(HReactor *reactor){mReactor = reactor;}
    EIOStat GetIOStat()const{return mIOStat;}
    void SetIOStat(EIOStat stat){mIOStat=stat;}
protected:
    handle_t mHandle=-1;
    HReactor *mReactor=nullptr;
private:
    bool mNeedClosed = false;
    EIOStat mIOStat = EIOStat::None;
};
