#include "HuolunCore/HPlatform.h"
#include "HuolunCore/Reactor/HReactorFactory.h"
#include "HuolunCore/Reactor/HKqueueReactor.h"
#include "HuolunCore/Reactor/HEpollReactor.h"
#include "HuolunCore/Reactor/HPollReactor.h"
#include "HuolunCore/Reactor/HSelectReactor.h"
#include "HuolunCore/Reactor/HIOCPReactor.h"
HReactor *HReactorFactory::CreateReactor()
{
#if defined(__HUOLUN_KQUEUE__)
    return Create<HKqueueReactor>();
#elif defined(__HUOLUN_EPOLL__) 
    return Create<HEpollReactor>();
#endif
    return nullptr;
}

HReactorFactory *HReactorFactory::GetFactory()
{
    ReactorType type=None;
#if defined(__HUOLUN_KQUEUE__)
    type=KQUEUE;
#elif defined(__HUOLUN_EPOLL__)
    type=EPOLL;
#elif defined(__HUOLUN_IOCP__)
    type=IOCP;
#elif defined(__HUOLUN_SELECT__)
    type=SELECT;

#elif defined(__HUOLUN_POLL__)
    type=POLL;
#endif
    return GetFactory(type);
}
HReactorFactory *HReactorFactory::GetFactory(ReactorType type)
{
    HReactorFactory *ret=nullptr;
    switch(type)
    {
#if defined(__HUOLUN_KQUEUE__)
    case KQUEUE:
        ret = Create<HKqueueReactorFactory>();
        break;
#endif
#if defined(__HUOLUN_EPOLL__)
    case EPOLL:
        ret = Create<HEpollReactorFactory>();
        break;
#endif
#if defined(__HUOLUN_SELECT__)
    case SELECT:
        ret = Create<HSelectReactorFactory>();
        break;
#endif
#if defined(__HUOLUN_POLL__)
    case POLL:
        ret = Create<HPollReactorFactory>();
        break;
#endif
#if defined(__HUOLUN_IOCP__)
    case IOCP:
        ret = Create<HIOCPReactorFactory>();
        break;
#endif
    default:
        break;
    }
    return ret;
}
