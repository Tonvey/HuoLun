#pragma once
#include "HLayer.h"
#include "HIOChannel.h"
#include <list>
class HReactor;
class HBuffer;
class HComponent
    :virtual public HLayer
    ,virtual public HIOChannel
{
public:
    virtual bool OnRead(HBuffer *buffer)=0;
    virtual bool OnWrite(const HBuffer *buffer)=0;
    virtual HMessage *HandleMessageForward(HMessage *msg)override;
    virtual HMessage *HandleMessageBackward(HMessage *msg)override;
public:
    HComponent();
    ~HComponent();
    virtual void TriggerRead()override;
    virtual void TriggerWrite()override;
    void PushBufferBack(HBuffer *buf);
    void PopBufferFront();
    void ResetBuffer();
private:
    void SetWriteFlag();
    void ClearWriteFlag();
    std::list<HBuffer*> mBuffers;
};
