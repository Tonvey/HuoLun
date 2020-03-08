#include "Huolun.h"
#include "HuolunCore/HTCPListenerComponent.h"
#include "HuolunCore/HTCPDataComponent.h"
#include "util/EchoLayer.hpp"
#include <iostream>
using namespace std;
class MyLayerFactory
    :public HLayerFactory
{
public:
    virtual HLayer *CreateLayer(HTCPDataComponent *dataComp)
    {
        auto echo = CreateObject<EchoLayer>();
        echo->SetDstLayer(dataComp);
        return echo;
    }
};
int main()
{
    Huolun core;
    if(!core.Initialize())
    {
        cerr<<"Huolun init fail"<<endl;
        return -1;
    }
    auto fact = CreateObject<MyLayerFactory>();
    auto listen = CreateObject<HTCPListenerComponent>(8899,fact);
    core.RegisterChannel(listen);
    fact->Release();
    listen->Release();
    core.Run();
    core.Finish();
    return 0;
}
