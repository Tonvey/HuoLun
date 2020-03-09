#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/Reactor/HPollReactor.h"
#include "HuolunCore/HIOChannel.h"
#ifdef __HUOLUN_POLL__
#include <unistd.h>
#include <sys/errno.h>
#include <poll.h>
#include <vector>
#include <list>
#include <cstring>
typedef std::vector<pollfd> pollfdvec_t;
typedef std::vector<HIOChannel*> channelvec_t;
HPollReactor::HPollReactor()
{
}
HPollReactor::~HPollReactor()
{
    Stop();
    Finish();
}
bool HPollReactor::RegisterReadImpl(HIOChannel *ch)
{
    return true;
}
bool HPollReactor::UnregisterReadImpl(HIOChannel *ch)
{
    return true;
}
bool HPollReactor::RegisterWriteImpl(HIOChannel *ch)
{
    return true;
}
bool HPollReactor::UnregisterWriteImpl(HIOChannel *ch)
{
    return true;
}
void HPollReactor::Run()
{
    mRunningFlag=RunningFlag::Running;
    while (mRunningFlag == Running)
    {
        pollfdvec_t fds(mSetOfChannel.size());
        channelvec_t channels(mSetOfChannel.size());
        for(auto it : mSetOfChannel)
        {
            pollfd pfd;
            std::memset(&pfd,0,sizeof(pfd));
            pfd.fd = it->GetHandle();
            if(it->GetIOStat()&EIOStat::In)
            {
                pfd.events|=POLLIN;
            }
            if(it->GetIOStat()&EIOStat::Out)
            {
                pfd.events|=POLLOUT;
            }
            fds.push_back(pfd);
            channels.push_back(it);
        }

        int ret = poll(fds.data(),fds.size(),-1);
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
        for(int i = 0 ;i<fds.size();++i)
        {
            auto itfd = fds[i];
            auto itCh = channels[i];
            if(itfd.revents&POLLIN)
            {
                itCh->TriggerRead();
                if (true == itCh->IsNeedClosed())
                {
                    uninstallList.push_back(itCh);
                    continue;
                }
            }
            if(itfd.revents&POLLOUT)
            {
                itCh->TriggerWrite();
                if (true == itCh->IsNeedClosed())
                {
                    uninstallList.push_back(itCh);
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
void HPollReactor::Stop()
{
    mRunningFlag = RunningFlag::Initialized;
}
HReactor *HPollReactorFactory::CreateReactor()
{
    return Create<HPollReactor>();
}
#endif
