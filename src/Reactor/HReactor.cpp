#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/HIOChannel.h"
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
