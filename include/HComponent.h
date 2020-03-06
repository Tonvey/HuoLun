#pragma once
#include "HuoLun.h"
#include <list>
#include <queue>
#include <string>
class HLayer;
class HComponent
    :public HObject
{
public:
    typedef std::queue<HLayer*,std::list<HLayer*>> layer_queue_t;
    virtual bool Initialize()=0;
    virtual bool Finish()=0;
    virtual handle_t GetHandle()=0;
    virtual bool OnRead(std::string &readContent)=0;
    virtual bool OnWrite(const std::string &writeContent)=0;
    virtual bool OnError(const std::string &error)=0;
public:
    virtual bool PushLayer(HLayer *layer)=0;
    virtual HLayer *PopLayerFront();
    virtual HLayer *PopLayerBack();
    virtual void ResetQueue();
    virtual int QueueSize();
private:
    std::queue<HLayer*,std::list<HLayer*>> mLayers;
};
