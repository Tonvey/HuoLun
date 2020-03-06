#pragma once
#include <map>
#include "HTypes.h"
class HLayer;
class HComponent;
class Huolun
{
public:
    Huolun();
    virtual ~Huolun();
    // Call this function 
    bool Initialize();
    // Call this function after all of your jobs done , this will clean the core context
    bool Finish();
    // return handle
    bool AddComponent(HComponent *comp);
    bool RemoveComponent(handle_t handle);
    // Will run until call stop 
    void Run();
    void Stop();
private:
    handle_t mCoreHandle;
    //TODO : Container choose optimize
    std::map<handle_t,HComponent*> mMapOfHandleComponent;
    enum RunningFlag
    {
        Created,
        Initialized,
        Running,
    } mRunningFlag;
};
