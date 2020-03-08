#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/HIOChannel.h"
HReactor::HReactor()
{
}
HReactor::~HReactor()
{
}
bool HReactor::Initialize()
{
    return true;
}
void HReactor::Finish()
{
    for (auto itr : mMapOfHandleChannel)
    {
        auto ch = itr.second;
        ch->Finish();
        ch->Release();
    }
    mMapOfHandleChannel.clear();
    mSetOfChannel.clear();
    mRunningFlag = RunningFlag::Created;
}
bool HReactor::RegisterRead(HIOChannel *ch)
{
    if(ch->GetIOStat()&EIOStat::In)
        return true;
    if(RegisterReadImpl(ch))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()|EIOStat::In));
        return true;
    }
    return false;
}
bool HReactor::UnregisterRead(HIOChannel *ch)
{
    if((ch->GetIOStat()&EIOStat::In)==0)
        return true;
    if(UnregisterReadImpl(ch))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()&~EIOStat::In));
        return true;
    }
    return false;
}
bool HReactor::RegisterWrite(HIOChannel *ch)
{
    if(ch->GetIOStat()&EIOStat::Out)
        return true;
    if(RegisterWriteImpl(ch))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()|EIOStat::Out));
        return true;
    }
    return false;
}
bool HReactor::UnregisterWrite(HIOChannel *ch)
{
    if((ch->GetIOStat()&EIOStat::Out)==0)
        return true;
    if(UnregisterWriteImpl(ch))
    {
        ch->SetIOStat(EIOStat(ch->GetIOStat()&~EIOStat::Out));
        return true;
    }
    return false;
}

bool HReactor::Install(HIOChannel *ch)
{
    bool isInstalled = false;
    if(mSetOfChannel.find(ch)!=mSetOfChannel.end())
    {
        isInstalled=true;
    }
    if(!isInstalled&&ch->Initialize())
    {
        mMapOfHandleChannel.insert(std::make_pair(ch->GetHandle(),ch));
        mSetOfChannel.insert(ch);
        ch->Retain();
        ch->SetReactor(this);
    }
    RegisterRead(ch);
    return isInstalled;
}
bool HReactor::Uninstall(handle_t handle)
{
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

bool HReactor::Uninstall(HIOChannel *ch)
{
    return Uninstall(ch->GetHandle());
}
