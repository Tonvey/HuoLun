#include "HuolunCore/HComponent.h"
#include "HuolunCore/HLayer.h"
#include "HuolunCore/Reactor/HReactor.h"
#include <cassert>
#include <iostream>
using namespace std;

HComponent::HComponent()
{
}
HComponent::~HComponent()
{
    ResetBuffer();
}
void HComponent::PushBufferBack(HBuffer *buf)
{
    cout<<"Push Buffer back"<<endl;
    assert(buf);
    buf->Retain();
    mBuffers.push_back(buf);
    SetWriteFlag();
}
void HComponent::PopBufferFront()
{
    auto element = mBuffers.front();
    mBuffers.pop_front();
    element->Release();
    SetWriteFlag();
}
void HComponent::ResetBuffer()
{
    for(auto i : mBuffers)
    {
        i->Release();
    }
    mBuffers.clear();
}
void HComponent::TriggerRead()
{
    Handle(HandleDirection::Forward,nullptr);
}
void HComponent::SetWriteFlag()
{
    assert(mReactor);
    mReactor->RegisterWrite(this);
}
void HComponent::ClearWriteFlag()
{
    assert(mReactor);
    mReactor->UnregisterWrite(this);
}
void HComponent::TriggerWrite()
{
    while(mBuffers.size()>0)
    {
        auto buf = mBuffers.front();
        if(OnWrite(buf))
        {
            PopBufferFront();
        }
        else
        {
            break;
        }
    }
    if(mBuffers.size()==0)
    {
        ClearWriteFlag();
    }
}
HMessage *HComponent::HandleMessageForward(HMessage *msg)
{
    HBytesMessage *ret = Create<HBytesMessage>();
    if(!OnRead(ret->GetBytes()))
    {
        ret->Release();
        ret=nullptr;
    }
    return ret;
}
HMessage *HComponent::HandleMessageBackward(HMessage *msg)
{
    auto bytes = dynamic_cast<HBytesMessage*>(msg);
    if(bytes!=nullptr)
    {
        PushBufferBack(bytes->GetBytes());
    }
    return nullptr;
}
