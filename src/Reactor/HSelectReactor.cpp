#include "HuolunCore/Reactor/HSelectReactor.h"
#include "HuolunCore/HIOChannel.h"
#include <unistd.h>
#include <sys/errno.h>
#include <sys/select.h>
#include <list>
#include <iostream>
HSelectReactor::HSelectReactor()
{
}
HSelectReactor::~HSelectReactor()
{
    Stop();
    Finish();
}
bool HSelectReactor::RegisterReadImpl(HIOChannel *ch)
{
    return true;
}
bool HSelectReactor::UnregisterReadImpl(HIOChannel *ch)
{
    return true;
}
bool HSelectReactor::RegisterWriteImpl(HIOChannel *ch)
{
    return true;
}
bool HSelectReactor::UnregisterWriteImpl(HIOChannel *ch)
{
    return true;
}
void HSelectReactor::Run()
{
    mRunningFlag=RunningFlag::Running;
    fd_set rset,wset;
    while (mRunningFlag == Running)
    {
        //Init data set
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        for(auto it : mSetOfChannel)
        {
            if(it->GetIOStat()&EIOStat::In)
            {
                FD_SET(it->GetHandle(),&rset);
            }
            if(it->GetIOStat()&EIOStat::Out)
            {
                FD_SET(it->GetHandle(),&wset);
            }
        }

        int ret = select(FD_SETSIZE,&rset,&wset,nullptr,nullptr);
        if (ret<=0)
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
        for(auto itChannel : mSetOfChannel)
        {
            if(FD_ISSET(itChannel->GetHandle(),&rset))
            {
                itChannel->TriggerRead();
                if (true == itChannel->IsNeedClosed())
                {
                    uninstallList.push_back(itChannel);
                    continue;
                }
            }
            if(FD_ISSET(itChannel->GetHandle(),&wset))
            {
                itChannel->TriggerWrite();
                if (true == itChannel->IsNeedClosed())
                {
                    uninstallList.push_back(itChannel);
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
void HSelectReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
HReactor *HSelectReactorFactory::CreateReactor()
{
    return Create<HSelectReactor>();
}
