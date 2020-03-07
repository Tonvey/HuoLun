#ifdef __HUOLUN_PLATFORM_APPLE__
#include "HuolunCore/Reactor/HKqueueReactor.h"
#include <unistd.h>
#include <sys/errno.h>
#include <sys/event.h>
HKqueueReactor::HKqueueReactor()
{
    mRunningFlag = Created;
}
HKqueueReactor::~HKqueueReactor()
{
}
bool HKqueueReactor::Initialize()
{
    if(mRunningFlag== RunningFlag::Created)
    {
        mCoreHandle = kqueue();
        mRunningFlag = RunningFlag::Initialized;
    }
    return mCoreHandle>=0;
}
bool HKqueueReactor::Finish()
{
    if(mRunningFlag==RunningFlag::Running)
    {
        return false;
    }
    for (auto itr : mMapOfHandleChannel)
    {
        auto hcomp = itr.second;
        hcomp->Finish();
    }
    mMapOfHandleChannel.clear();
    if(mCoreHandle>=0)
    {
        close(mCoreHandle);
    }
    mRunningFlag = RunningFlag::Created;
    return true;
}
bool HKqueueReactor::Register(HIOChannel *ch,EIOStat flag=EIOStat::In)
{
    if(mRunningFlag<RunningFlag::Initialized)
    {
        return false;
    }
    handle_t h = comp->GetHandle();
    if(this->mMapOfHandleChannel.find(h)!=mMapOfHandleChannel.end())
    {
        // has add
        return true;
    }
    if(!comp->Initialize())
    {
        return false;
    }
    mMapOfHandleChannel.insert(std::make_pair(h,comp));
    comp->Retain();
    comp->SetCore(this);

    //kqueue
    bool ret = false;
    struct kevent stEvent;
    //kqueue timer
    //if(_oChannel.GetTimerInterval()>0)
    //{
    //    EV_SET(&stEvent, comp->GetHandle(),EVFILT_TIMER, EV_ADD , 0,_oChannel.GetTimerInterval(), comp);
    //}
    //else
    //{
    EV_SET(&stEvent, comp->GetHandle(),EVFILT_READ, EV_ADD , 0, 0, comp);
    //}
    if(kevent(mCoreHandle, &stEvent, 1, NULL, 0, NULL)>0)
    {
        //add to map
        ret = true;
    }
    return ret;
}
bool HKqueueReactor::Unregister(handle_t handle)
{
    if(mRunningFlag<RunningFlag::Initialized)
    {
        return false;
    }
    auto it = this->mMapOfHandleChannel.find(handle);
    auto comp = it->second;
    if(it==mMapOfHandleChannel.end())
    {
        // has add
        return true;
    }
    mMapOfHandleChannel.erase(it);

    // kqueue
    struct kevent evt;
    //timer check
    //if(comp->GetTimerInterval()>0)
    //{
    //    EV_SET(&evt, comp->GetHandle(),EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
    //}
    //else
    //{
    EV_SET(&evt, comp->GetHandle(),EVFILT_READ, EV_DELETE, 0, 0, NULL);
    //}
    kevent(mCoreHandle, &evt, 1, NULL, 0, NULL);
    comp->SetCore(nullptr);
    it->second->Release();
    return true;
}
void HKqueueReactor::Run()
{
    cout<<__LINE__<<endl;
    if(mRunningFlag!=RunningFlag::Initialized)
    {
        cout<<__LINE__<<endl;
        return;
    }
    else
    {
        cout<<__LINE__<<endl;
        mRunningFlag=RunningFlag::Running;
    }
    int iEpollRet = -1;
    cout<<__LINE__<<endl;
    while (mRunningFlag == Running)
    {
        struct kevent atmpEvent[10];
        iEpollRet =kevent(mCoreHandle, NULL, 0, atmpEvent, 10, NULL);
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
                    this->UnregisterChannel(poChannel->GetHandle());
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
            //else if (EVFILT_WRITE == atmpEvent[i].filter)
            //{
            //    poChannel->FlushOut();
            //    if (false == poChannel->HasOutput())
            //    {
            //        Zinx_ClearChannelOut(*poChannel);
            //    }
            //}
        }
    }
}
void HKqueueReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
#endif
