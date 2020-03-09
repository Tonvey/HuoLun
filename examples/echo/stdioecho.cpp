#include "Huolun.h"
#include "util/StdinComponent.hpp"
#include "util/StdoutComponent.hpp"
#include "util/EchoLayer.hpp"
#include <iostream>
using namespace std;
int main()
{
#ifdef __HUOLUN_PLATFORM_WIN__
    cerr << "Windows not support standard IO component" << endl;
#endif
    Huolun core;
    if(!core.Initialize())
    {
        cerr<<"Huolun init fail"<<endl;
        return -1;
    }
    auto incomp = HObject::Create<StdinComponent>();
    auto echoLayer = HObject::Create<EchoLayer>();
    auto outcomp = HObject::Create<StdoutComponent>();
    incomp->SetNextHandler(echoLayer);
    echoLayer->SetDstLayer(outcomp);
    core.RegisterChannel(incomp);
    core.RegisterChannel(outcomp);
    core.Run();
    core.Finish();
    incomp->Release();
    echoLayer->Release();
    outcomp->Release();
    return 0;
}
