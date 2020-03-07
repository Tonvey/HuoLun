#pragma once
#include "HMessage.h"
class HMessageHandler
    :virtual public HObject
{
public:
    enum HandleDirection
    {
        Forward,    //Call next
        Backward    //Call preivious
    };
    void Handle(HandleDirection direction,HMessage *msg)
    {
        if(direction==HandleDirection::Forward)
        {
            auto nextMsg = HandleMessageForward(msg);
            if(GetNextHandler()!=nullptr)
            {
                GetNextHandler()->Handle(direction,msg);
                nextMsg->Release();
            }
        }
        if(direction==HandleDirection::Backward)
        {
            auto nextMsg = HandleMessageBackward(msg);
            if(GetPrevHandler()!=nullptr)
            {
                GetPrevHandler()->Handle(direction,msg);
                nextMsg->Release();
            }
        }
    }
    //Override these two function to handle message in/out
    virtual HMessage *HandleMessageForward(HMessage *msg)=0;
    virtual HMessage *HandleMessageBackward(HMessage *msg)=0;
public:
    HMessageHandler *GetNextHandler()const{return mNextHandler;};
    HMessageHandler *GetPrevHandler()const{return mPrevHandler;};
    void SetNextHandler(HMessageHandler *handler){mNextHandler=handler;};
    void SetPrevHandler(HMessageHandler *handler){mPrevHandler=handler;};
private:
    HMessageHandler *mNextHandler=nullptr;
    HMessageHandler *mPrevHandler=nullptr;
};
