#include "HuolunCore/Huolun.h"
#include "HuolunCore/HIOChannel.h"
#include "HuolunCore/Reactor/HReactorFactory.h"
#include <cassert>
#include <iostream>
using namespace std;
Huolun::Huolun()
{
    std::string errBuf;
    auto fact = HReactorFactory::GetFactory();
    mReactor = fact->CreateReactor();
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
void Huolun::Finish()
{
    assert(mReactor);
    mReactor->Finish();
}
bool Huolun::RegisterChannel(HIOChannel *comp)
{
    assert(mReactor);
    return mReactor->Install(comp);
}
bool Huolun::UnregisterChannel(reactor_handle_t handle)
{
    assert(mReactor);
    return mReactor->Uninstall(handle);
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
