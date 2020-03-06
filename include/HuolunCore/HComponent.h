#pragma once
#include "HObject.h"
#include "HLayer.h"
#include <list>
#include <string>
class HLayer;
class HComponent
    :public HObject
{
public:
    virtual bool Initialize()=0;
    virtual bool Finish()=0;
    virtual handle_t GetHandle(){return mHandle;};
    virtual bool OnRead(std::string &readContent)=0;
    virtual bool OnWrite(const std::string &writeContent)=0;
    virtual void OnError(const std::string &error)=0;
    virtual bool IsNeedClosed(){return mNeedClosed;};
public:
    HComponent();
    ~HComponent();
    bool PushLayerBack(HLayer *layer);
    bool PushLayerFront(HLayer *layer);
    HLayer *PopLayerFront();
    HLayer *PopLayerBack();
    void ResetQueue();
    int QueueSize();
private:
    std::list<HLayer*> mLayers;
protected:
    bool mNeedClosed ;
    handle_t mHandle;
};
