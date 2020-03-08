#pragma once
#include <map>
#include <functional>
template<class T,class ...Args>
class HEvent
{
public:
    typedef std::function<T(Args...)> callback_t;
    typedef uint32_t callback_handle_t;
    void Broadcast(Args... args)
    {
        for(auto &pair:mMapOfCallback)
        {
            pair.second(args...);
        }
    }
    callback_handle_t Bind(callback_t fn)
    {
        mMapOfCallback.insert(std::make_pair(mHandleCount,fn));
        return mHandleCount++;
    }
    void Unbind(callback_handle_t handle)
    {
        auto it = mMapOfCallback.find(handle);
        if(it!=mMapOfCallback.end())
        {
            mMapOfCallback.erase(it);
        }
    }
    void Clear()
    {
        mMapOfCallback.clear();
    }
private:
    std::map<callback_handle_t,callback_t> mMapOfCallback;
    callback_handle_t mHandleCount=0;
};
