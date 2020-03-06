#include "HuolunCore/Huolun.h"
#include "HuolunCore/HComponent.h"
#include <unistd.h>
#include <sys/errno.h>
#include <sys/event.h>
#include <iostream>
using namespace std;
Huolun::Huolun()
{
    mRunningFlag = Created;
}
Huolun::~Huolun()
{
    Finish();
}
bool Huolun::Initialize()
{
    if(mRunningFlag== RunningFlag::Created)
    {
        mCoreHandle = kqueue();
        mRunningFlag = RunningFlag::Initialized;
    }
    return mCoreHandle>=0;
}
bool Huolun::Finish()
{
    if(mRunningFlag==RunningFlag::Running)
    {
        return false;
    }
    for (auto itr : mMapOfHandleComponent)
    {
        auto hcomp = itr.second;
        hcomp->Finish();
    }
    mMapOfHandleComponent.clear();
    if(mCoreHandle>=0)
    {
        close(mCoreHandle);
    }
    mRunningFlag = RunningFlag::Created;
    return true;
}
bool Huolun::AddComponent(HComponent *comp)
{
    if(mRunningFlag<RunningFlag::Initialized)
    {
        return false;
    }
    handle_t h = comp->GetHandle();
    if(this->mMapOfHandleComponent.find(h)!=mMapOfHandleComponent.end())
    {
        // has add
        return true;
    }
    if(!comp->Initialize())
    {
        return false;
    }
    mMapOfHandleComponent.insert(std::make_pair(h,comp));
    comp->Retain();

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
bool Huolun::RemoveComponent(handle_t handle)
{
    if(mRunningFlag<RunningFlag::Initialized)
    {
        return false;
    }
    auto it = this->mMapOfHandleComponent.find(handle);
    auto comp = it->second;
    if(it==mMapOfHandleComponent.end())
    {
        // has add
        return true;
    }
    mMapOfHandleComponent.erase(it);

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
    it->second->Release();
    return true;
}

void Huolun::Run()
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
            HComponent *poChannel = static_cast<HComponent *>(atmpEvent[i].udata);
            if (EVFILT_READ == atmpEvent[i].filter)
            {
                std::string in;
                poChannel->OnRead(in);
                std::cout<<in<<std::endl;
                if (true == poChannel->IsNeedClosed())
                {
                    this->RemoveComponent(poChannel->GetHandle());
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
void Huolun::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
