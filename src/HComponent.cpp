#include "HComponent.h"

bool HComponent::PushLayer(HLayer *layer)
{
    assert(layer);
    layer->retain();
    mLayers.push_back(layer);
}
HLayer *HComponent::PopLayerFront()
{
    auto ret = mLayers.front();
    mLayers.pop_front();
    //TODO : need release ?
    ret.release();
    return ret;
}
HLayer *HComponent::PopLayerBack()
{
    auto ret = mLayers.back();
    mLayers.pop_back();
    ret.release();
    return ret;
}
void HComponent::ResetQueue()
{
    for(auto i : mLayers)
    {
        i->release();
    }
    mLayers.swap(layer_queue_t());
}
int HComponent::QueueSize()
{
    return mLayers.size();
}
