#pragma once
#include "HReactor.h"
#include <string>
class HReactorFactory
    :public HObject
{
public:
    enum ReactorType
    {
        SELECT,
        POLL,
        EPOLL,
        KQUEUE,
        IOCP
    };
    static HReactor *CreateReactor(std::string &errStr);
    static HReactor *CreateReactor(ReactorType type,std::string &errStr);
};
