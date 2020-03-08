#pragma once
#include "HReactor.h"
#include <string>
class HComponent;
class HReactorFactory
    :public HObject
{
public:
    enum ReactorType
    {
        None,
#ifdef __HUOLUN_SELECT__
        SELECT,
#endif
#ifdef __HUOLUN_POLL__
        POLL,
#endif
#ifdef __HUOLUN_EPOLL__
        EPOLL,
#endif
#ifdef __HUOLUN_KQUEUE__
        KQUEUE,
#endif
#ifdef __HUOLUN_IOCP__
        IOCP
#endif
    };
    virtual HReactor *CreateReactor()=0;
public:
    static HReactorFactory *GetFactory();
    static HReactorFactory *GetFactory(ReactorType type);
private:
    ReactorType mType;
};
