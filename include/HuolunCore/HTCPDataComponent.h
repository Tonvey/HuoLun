#pragma once
#include "HComponent.h"
#include <array>
typedef std::array<uint8_t,4> ipv4_t;
class HTCPDataComponent
    :virtual public HComponent
{
public:
    virtual bool Initialize()override;
    virtual void Finish()override;
    HTCPDataComponent(handle_t handle,std::array<uint8_t,4> ip,uint16_t port);
    ~HTCPDataComponent();
public:
    virtual bool OnRead(HBuffer *buffer)override;
    virtual bool OnWrite(const HBuffer *buffer)override;
private:
    ipv4_t mDestIp={0};
    uint16_t mDestPort = 0;
};
