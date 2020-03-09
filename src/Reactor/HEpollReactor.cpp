#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/Reactor/HEpollReactor.h"
#include "HuolunCore/HIOChannel.h"
#ifdef __HUOLUN_EPOLL__
#include <unistd.h>
#include <sys/errno.h>
#include <sys/epoll.h>
#include <list>
HEpollReactor::HEpollReactor()
{
}
HEpollReactor::~HEpollReactor()
{
    Stop();
    Finish();
}
bool HEpollReactor::Initialize()
{
    HReactor::Initialize();
    mEpfd = epoll_create(1);
    return mEpfd>=0;
}
void HEpollReactor::Finish()
{
    HReactor::Finish();
    if(mEpfd>=0)
    {
        close(mEpfd);
        mEpfd=-1;
    }
}
bool HEpollReactor::RegisterReadImpl(HIOChannel *ch)
{
    int op = EPOLL_CTL_MOD;
    if(ch->GetIOStat()==None)
    {
        op = EPOLL_CTL_ADD;
    }
    int events = EPOLLIN;
    events |= (ch->GetIOStat()&EIOStat::Out)?EPOLLOUT:0;
    return EpollCtl(ch,op,events);
}
bool HEpollReactor::UnregisterReadImpl(HIOChannel *ch)
{
    int op = EPOLL_CTL_MOD;
    int events = 0;
    events |= (ch->GetIOStat()&EIOStat::Out)?EPOLLOUT:0;
    return EpollCtl(ch,op,events);
}
bool HEpollReactor::RegisterWriteImpl(HIOChannel *ch)
{
    int op = EPOLL_CTL_MOD;
    if(ch->GetIOStat()==None)
    {
        op = EPOLL_CTL_ADD;
    }
    int events = EPOLLOUT;
    events |= (ch->GetIOStat()&EIOStat::In)?EPOLLIN:0;
    return EpollCtl(ch,op,events);
}
bool HEpollReactor::UnregisterWriteImpl(HIOChannel *ch)
{
    int op = EPOLL_CTL_MOD;
    int events = 0;
    events |= (ch->GetIOStat()&EIOStat::In)?EPOLLIN:0;
    return EpollCtl(ch,op,events);
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
void HEpollReactor::Run()
{
    mRunningFlag=RunningFlag::Running;
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
        std::list<HIOChannel*> uninstallList;
        for (int i = 0; i < iEpollRet; i++)
        {
            HIOChannel *poChannel = static_cast<HIOChannel*>(atmpEvent[i].data.ptr);
            if (0 != (EPOLLIN & atmpEvent[i].events))
            {
                poChannel->TriggerRead();
                if (true == poChannel->IsNeedClosed())
                {
                    uninstallList.push_back(poChannel);
                    continue;
                }
            }
            if (0 != (EPOLLOUT & atmpEvent[i].events))
            {
                poChannel->TriggerWrite();
                if (true == poChannel->IsNeedClosed())
                {
                    uninstallList.push_back(poChannel);
                    continue;
                }
            }
        }
        for(auto itChannel : uninstallList)
        {
            Uninstall(itChannel);
        }
    }

}
void HEpollReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
HReactor *HEpollReactorFactory::CreateReactor()
{
    return Create<HEpollReactor>();
}
#endif
