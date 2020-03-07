#include "HuolunCore/HPlatform.h"
#include "HuolunCore/Reactor/HReactorFactory.h"

HReactor *HReactorFactory::CreateReactor(std::string &errStr)
{
#if defined(__HUOLUN_KQUEUE__)
#endif
    errStr = "Unknowed Platform";
    return nullptr;
}
HReactor *HReactorFactory::CreateReactor(HReactorFactory::ReactorType type,std::string &errStr)
{
    errStr = "Unknowed Platform";
    return nullptr;
}

