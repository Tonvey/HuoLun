#include "HComponent.h"
#include "HLayer.h"

HComponent::~HComponent()
{
    ResetQueue();
}
bool HComponent::PushLayerBack(HLayer *layer)
{
    assert(layer);
    layer->Retain();
    mLayers.push_back(layer);
}
bool HComponent::PushLayerFront(HLayer *layer)
{
    assert(layer);
    layer->Retain();
    mLayers.push_front(layer);
}
HLayer *HComponent::PopLayerFront()
{
    auto ret = mLayers.front();
    mLayers.pop_front();
    //TODO : need release ?
    ret->Release();
    return ret;
}
HLayer *HComponent::PopLayerBack()
{
    auto ret = mLayers.back();
    mLayers.pop_back();
    ret->Release();
    return ret;
}
void HComponent::ResetQueue()
{
    for(auto i : mLayers)
    {
        i->Release();
    }
    mLayers.clear();
}
int HComponent::QueueSize()
{
    return mLayers.size();
}
