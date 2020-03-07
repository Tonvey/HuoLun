#include "HuolunCore/Huolun.h"
#include "HuolunCore/HIOChannel.h"
#include "HuolunCore/Reactor/HReactorFactory.h"
#include <iostream>
using namespace std;
Huolun::Huolun()
{
    std::string errBuf;
    mReactor = HReactorFactory::CreateReactor(errBuf);
    assert(mReactor);
}
Huolun::~Huolun()
{
    Finish();
}
bool Huolun::Initialize()
{
    assert(mReactor);
    return mReactor->Initialize();
}
bool Huolun::Finish()
{
    assert(mReactor);
    return mReactor->Finish();
}
bool Huolun::RegisterChannel(HIOChannel *comp)
{
    assert(mReactor);
    return mReactor->Register(comp);
}
bool Huolun::UnregisterChannel(handle_t handle)
{
    assert(mReactor);
    return mReactor->Unregister(handle);
}

void Huolun::Run()
{
    assert(mReactor);
    mReactor->Run();
}
void Huolun::Stop()
{
    assert(mReactor);
    mReactor->Stop();
}
