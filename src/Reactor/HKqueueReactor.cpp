#include "HuolunCore/Reactor/HKqueueReactor.h"
#include "HuolunCore/HIOChannel.h"
#ifdef __HUOLUN_KQUEUE__
#include <unistd.h>
#include <sys/errno.h>
#include <sys/event.h>
#include <list>
HKqueueReactor::HKqueueReactor()
{
}
HKqueueReactor::~HKqueueReactor()
{
    Stop();
    Finish();
}
bool HKqueueReactor::Initialize()
{
    HReactor::Initialize();
    mKqueueFd = kqueue();
    return mKqueueFd>=0;
}
void HKqueueReactor::Finish()
{
    HReactor::Finish();
    if(mKqueueFd>=0)
    {
        close(mKqueueFd);
        mKqueueFd=-1;
    }
}
bool HKqueueReactor::RegisterReadImpl(HIOChannel *ch)
{
    return KqueueModFilter(ch,EVFILT_READ,EV_ADD);
}
bool HKqueueReactor::UnregisterReadImpl(HIOChannel *ch)
{
    return KqueueModFilter(ch,EVFILT_READ,EV_DELETE);
}
bool HKqueueReactor::RegisterWriteImpl(HIOChannel *ch)
{
    return KqueueModFilter(ch,EVFILT_WRITE,EV_ADD);
}
bool HKqueueReactor::UnregisterWriteImpl(HIOChannel *ch)
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
void HKqueueReactor::Run()
{
    mRunningFlag=RunningFlag::Running;
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
        std::list<HIOChannel*> uninstallList;
        for (int i = 0; i < iEpollRet; i++)
        {
            HIOChannel *poChannel = static_cast<HIOChannel *>(atmpEvent[i].udata);
            if (EVFILT_READ == atmpEvent[i].filter)
            {
                poChannel->TriggerRead();
                if (true == poChannel->IsNeedClosed())
                {
                    uninstallList.push_back(poChannel);
                    continue;
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
void HKqueueReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
HReactor *HKqueueReactorFactory::CreateReactor()
{
    return Create<HKqueueReactor>();
}
#endif
