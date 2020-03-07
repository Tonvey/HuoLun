#include "Huolun.h"
#include "HuolunCore/StdInComponent.hpp"
#include <iostream>
using namespace std;
int main()
{
    Huolun core;
    if(!core.Initialize())
    {
        cerr<<"Huolun init fail"<<endl;
        return -1;
    }
    auto comp = StdInComponent::Create<StdInComponent>();
    core.RegisterChannel(comp);
    core.Run();
    core.Finish();
    return 0;
}
