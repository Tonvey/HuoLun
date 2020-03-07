#include "HuolunCore/Reactor/HEpollReactor.h"
#include "HuolunCore/HIOChannel.h"
#ifdef __HUOLUN_PLATFORM_LINUX__
#include <unistd.h>
#include <sys/errno.h>
#include <sys/epoll.h>
HEpollReactor::HEpollReactor()
{
    mRunningFlag = Created;
}
HEpollReactor::~HEpollReactor()
{
    Stop();
    Finish();
}
bool HEpollReactor::Initialize()
{
    if(mRunningFlag== RunningFlag::Created)
    {
        mEpfd = epoll_create(1);
        mRunningFlag = RunningFlag::Initialized;
    }
    return mEpfd>=0;
}
bool HEpollReactor::Finish()
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
    if(mEpfd>=0)
    {
        close(mEpfd);
        mEpfd=-1;
    }
    mRunningFlag = RunningFlag::Created;
    return true;
}
bool HEpollReactor::RegisterRead(HIOChannel *ch)
{
    if(ch->GetIOStat()&EIOStat::In)
        return true;
    int op = EPOLL_CTL_MOD;
    if(ch->GetIOStat()==None)
    {
        op = EPOLL_CTL_ADD;
    }
    int events = EPOLLIN;
    events |= (ch->GetIOStat()&EIOStat::Out)?EPOLLOUT:0;
    if(EpollCtl(ch,op,events))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()|EIOStat::In));
        return true;
    }
    return false;
}
bool HEpollReactor::UnregisterRead(HIOChannel *ch)
{
    if(ch->GetIOStat()&EIOStat::In==0)
        return true;
    int op = EPOLL_CTL_MOD;
    int events = 0;
    events |= (ch->GetIOStat()&EIOStat::Out)?EPOLLOUT:0;
    if(EpollCtl(ch,op,events))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()|EIOStat::In));
        return true;
    }
    return false;
}
bool HEpollReactor::RegisterWrite(HIOChannel *ch)
{
    if(ch->GetIOStat()&EIOStat::Out)
        return true;
    int op = EPOLL_CTL_MOD;
    if(ch->GetIOStat()==None)
    {
        op = EPOLL_CTL_ADD;
    }
    int events = EPOLLOUT;
    events |= (ch->GetIOStat()&EIOStat::In)?EPOLLIN:0;
    if(EpollCtl(ch,op,events))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()|EIOStat::Out));
        return true;
    }
    return false;
}
bool HEpollReactor::UnregisterWrite(HIOChannel *ch)
{
    if(ch->GetIOStat()&EIOStat::Out==0)
        return true;
    int op = EPOLL_CTL_MOD;
    int events = 0;
    events |= (ch->GetIOStat()&EIOStat::In)?EPOLLIN:0;
    if(EpollCtl(ch,op,events))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()|EIOStat::Out));
        return true;
    }
    return false;
}
bool HEpollReactor::EpollCtl(HIOChannel *ch,int op ,int events)
{
    bool ret = false;
    struct epoll_event stEvent;
    stEvent.events = events;
    stEvent.data.ptr = ch;
    if (0 == epoll_ctl(mEpfd,op , ch->GetHandle(), &stEvent))
    {
        ret = true;
    }
    return ret;
}
bool HEpollReactor::Install(HIOChannel *ch)
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
bool HEpollReactor::Uninstall(handle_t handle)
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
void HEpollReactor::Run()
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
        struct epoll_event atmpEvent[100];
        iEpollRet = epoll_wait(mEpfd, atmpEvent, 100, -1);
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
            HIOChannel *poChannel = static_cast<HIOChannel*>(atmpEvent[i].data.ptr);
            if (0 != (EPOLLIN & atmpEvent[i].events))
            {
                poChannel->TriggerRead();
                if (true == poChannel->IsNeedClosed())
                {
                    this->Uninstall(poChannel->GetHandle());
                    break;
                }
            }
            if (0 != (EPOLLOUT & atmpEvent[i].events))
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
void HEpollReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
#endif
