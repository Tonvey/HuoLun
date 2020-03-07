#include "HuolunCore/HComponent.h"
#include "HuolunCore/HLayer.h"
#include "HuolunCore/Reactor/HReactor.h"

HComponent::HComponent()
{
}
HComponent::~HComponent()
{
    ResetBuffer();
}
void HComponent::PushBufferBack(HBuffer *buf)
{
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
}
void HComponent::ClearWriteFlag()
{
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
