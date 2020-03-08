#include "Huolun.h"
#include "HuolunCore/HTCPListenerComponent.h"
#include "HuolunCore/HTCPDataComponent.h"
#include "util/EchoLayer.hpp"
#include <iostream>
using namespace std;
//Factory create the next layer of new socket data component
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
void usage(std::string argv0)
{
    cerr<<"Usage : "<<argv0<<" <bind port>"<<endl;
}
int main(int argc, char **argv)
{
    if(argc!=2)
    {
        usage(argv[0]);
        return -1;
    }
    uint16_t port=8899;
    port = std::atoi(argv[1]);
    Huolun core;
    if(!core.Initialize())
    {
        cerr<<"Huolun init fail"<<endl;
        return -1;
    }
    auto fact = CreateObject<MyLayerFactory>();
    auto listen = CreateObject<HTCPListenerComponent>(port,fact);
    core.RegisterChannel(listen);
    fact->Release();
    listen->Release();
    core.Run();
    core.Finish();
    return 0;
}
