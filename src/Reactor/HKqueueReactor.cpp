#include "HuolunCore/Reactor/HKqueueReactor.h"
#include "HuolunCore/HIOChannel.h"
#ifdef __HUOLUN_PLATFORM_APPLE__
#include <unistd.h>
#include <sys/errno.h>
#include <sys/event.h>
HKqueueReactor::HKqueueReactor()
{
    mRunningFlag = Created;
}
HKqueueReactor::~HKqueueReactor()
{
    Stop();
    Finish();
}
bool HKqueueReactor::Initialize()
{
    if(mRunningFlag== RunningFlag::Created)
    {
        mKqueueFd = kqueue();
        mRunningFlag = RunningFlag::Initialized;
    }
    return mKqueueFd>=0;
}
bool HKqueueReactor::Finish()
{
    if(mRunningFlag==RunningFlag::Running)
    {
        return false;
    }
    for (auto itr : mMapOfHandleChannel)
    {
        auto ch = itr.second;
        ch->Finish();
    }
    mMapOfHandleChannel.clear();
    if(mKqueueFd>=0)
    {
        close(mKqueueFd);
        mKqueueFd=-1;
    }
    mRunningFlag = RunningFlag::Created;
    return true;
}
bool HKqueueReactor::RegisterRead(HIOChannel *ch)
{
    return KqueueModFilter(ch,EVFILT_READ,EV_ADD);
}
bool HKqueueReactor::UnregisterRead(HIOChannel *ch)
{
    return KqueueModFilter(ch,EVFILT_READ,EV_DELETE);
}
bool HKqueueReactor::RegisterWrite(HIOChannel *ch)
{
    return KqueueModFilter(ch,EVFILT_WRITE,EV_ADD);
}
bool HKqueueReactor::UnregisterWrite(HIOChannel *ch)
{
    return KqueueModFilter(ch,EVFILT_WRITE,EV_DELETE);
}
bool HKqueueReactor::KqueueModFilter(HIOChannel *ch,int filter,int flag)
{
    bool ret = false;
    struct kevent stEvent;
    EV_SET(&stEvent, ch->GetHandle(),filter, flag , 0, 0, ch);
    if(kevent(mKqueueFd, &stEvent, 1, NULL, 0, NULL)==0)
    {
        ret = true;
    }
    else
    {
        ch->Finish();
    }
    return ret;
}
bool HKqueueReactor::Install(HIOChannel *ch)
{
    if(mRunningFlag<RunningFlag::Initialized)
    {
        return false;
    }
    bool isInstalled = false;
    if(mSetOfChannel.find(ch)!=mSetOfChannel.end())
    {
        isInstalled=true;
    }
    if(!isInstalled)
    {
        if(!ch->Initialize())
        {
            return false;
        }
        ch->Retain();
        ch->SetReactor(this);
    }
    RegisterRead(ch);
    return true;
}
bool HKqueueReactor::Uninstall(handle_t handle)
{
    if(mRunningFlag<RunningFlag::Initialized)
    {
        return false;
    }
    auto it = this->mMapOfHandleChannel.find(handle);
    if(it==mMapOfHandleChannel.end())
    {
        return true;
    }
    auto ch = it->second;
    mMapOfHandleChannel.erase(it);

    UnregisterRead(ch);
    UnregisterWrite(ch);

    ch->Finish();
    ch->SetReactor(nullptr);
    ch->Release();
    return true;
}
void HKqueueReactor::Run()
{
    if(mRunningFlag!=RunningFlag::Initialized)
    {
        return;
    }
    else
    {
        mRunningFlag=RunningFlag::Running;
    }
    int iEpollRet = -1;
    while (mRunningFlag == Running)
    {
        struct kevent atmpEvent[10];
        iEpollRet =kevent(mKqueueFd, NULL, 0, atmpEvent, 10, NULL);
        if (-1 == iEpollRet)
        {
            if (EINTR == errno)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        for (int i = 0; i < iEpollRet; i++)
        {
            HIOChannel *poChannel = static_cast<HIOChannel *>(atmpEvent[i].udata);
            if (EVFILT_READ == atmpEvent[i].filter)
            {
                poChannel->TriggerRead();
                if (true == poChannel->IsNeedClosed())
                {
                    this->Uninstall(poChannel->GetHandle());
                    break;
                }
            }
            //if(EVFILT_TIMER == atmpEvent[i].filter)
            //{
            //    poChannel->SetTimeOut( atmpEvent[i].data);
            //    SysIOReadyMsg IoStat = SysIOReadyMsg(SysIOReadyMsg::IN);
            //    poChannel->Handle(IoStat);
            //    if (true == poChannel->IsNeedClosed())
            //    {
            //        this->RemoveComponent(comp);
            //        break;
            //    }
            //}
            else if (EVFILT_WRITE == atmpEvent[i].filter)
            {
                poChannel->TriggerWrite();
                if (true == poChannel->IsNeedClosed())
                {
                    this->Uninstall(poChannel->GetHandle());
                    break;
                }
            }
        }
    }
}
void HKqueueReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
#endif
