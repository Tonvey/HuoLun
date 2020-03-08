#pragma once
#include "HComponent.h"
#include <array>
class HTCPDataComponent;
class HLayerFactory
    :public HObject
{
public:
    virtual HLayer *CreateLayer(HTCPDataComponent *dataComp)=0;
};
class HTCPListenerComponent
    :virtual public HComponent
{
public:
    HTCPListenerComponent(uint16_t port,HLayerFactory *fact);
    ~HTCPListenerComponent();
    virtual bool Initialize()override;
    virtual void Finish()override;
    // TODO ipv4 is useless , only bind 0.0.0.0
    void BindAddress(std::array<uint8_t,4> ipv4,uint16_t port);
public:
    virtual bool OnRead(HBuffer *buffer)override;
    virtual bool OnWrite(const HBuffer *buffer)override;
private:
    HLayerFactory *mLayerFact=nullptr;
    std::array<uint8_t,4> mIpv4={0};
    uint16_t mPort=0;
};
