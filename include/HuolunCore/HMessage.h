#pragma once
#include "HObject.h"
#include <string>
class HMessage
    :public HObject
{
};

class HBytesMessage
    :public HMessage
{
public:
    std::string bytes;
};
