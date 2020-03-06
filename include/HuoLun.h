#pragma once
#include <map>
class HLayer;
class HComponent;
typedef int handle_t;
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
private:
    handle_t mCoreHandle;
    //TODO : Container choose optimize
    std::map<handle_t,HComponent*> mMapOfHandle;
};
