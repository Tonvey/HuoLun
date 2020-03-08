#pragma once
#include "HTypes.h"
class HIOChannel;
class HReactor;
class Huolun
{
public:
    Huolun();
    virtual ~Huolun();
    // Call this function 
    bool Initialize();
    // Call this function after all of your jobs done , this will clean the core context
    void Finish();
    bool RegisterChannel(HIOChannel *comp);
    bool UnregisterChannel(handle_t handle);
    // Will run until call stop 
    void Run();
    void Stop();
private:
    HReactor *mReactor=nullptr;
};
