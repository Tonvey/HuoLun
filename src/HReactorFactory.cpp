#include "HuolunCore/HPlatform.h"
#include "HuolunCore/Reactor/HReactorFactory.h"
#include "HuolunCore/Reactor/HKqueueReactor.h"
#include "HuolunCore/Reactor/HEpollReactor.h"

HReactor *HReactorFactory::CreateReactor(std::string &errStr)
{
#if defined(__HUOLUN_KQUEUE__)
    return Create<HKqueueReactor>();
#elif defined(__HUOLUN_EPOLL__) 
    return Create<HEpollReactor>();
#endif
    errStr = "Unknowed Platform";
    return nullptr;
}
HReactor *HReactorFactory::CreateReactor(HReactorFactory::ReactorType type,std::string &errStr)
{
    errStr = "Unknowed Platform";
    return nullptr;
}

