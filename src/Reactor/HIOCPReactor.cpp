#include "HuolunCore/Reactor/HIOCPReactor.h"
#ifdef __HUOLUN_IOCP__
#include "HuolunCore/HIOChannel.h"
#include <list>
#include <iostream>
HIOCPReactor::HIOCPReactor()
{
}
HIOCPReactor::~HIOCPReactor()
{
    Stop();
    Finish();
}
bool HIOCPReactor::RegisterReadImpl(HIOChannel *ch)
{
    return true;
}
bool HIOCPReactor::UnregisterReadImpl(HIOChannel *ch)
{
    return true;
}
bool HIOCPReactor::RegisterWriteImpl(HIOChannel *ch)
{
    return true;
}
bool HIOCPReactor::UnregisterWriteImpl(HIOChannel *ch)
{
    return true;
}
void HIOCPReactor::Run()
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
                FD_SET(socket_handle_t(it->GetHandle()),&rset);
            }
            if(it->GetIOStat()&EIOStat::Out)
            {
                FD_SET(socket_handle_t(it->GetHandle()),&wset);
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
void HIOCPReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
HReactor *HIOCPReactorFactory::CreateReactor()
{
    return Create<HIOCPReactor>();
}
#endif
