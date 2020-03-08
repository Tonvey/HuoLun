#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/HTCPDataComponent.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;
HTCPDataComponent::HTCPDataComponent(handle_t handle,std::array<uint8_t,4> ip,uint16_t port)
    :mDestIp(ip)
    ,mDestPort(port)
{
    mHandle = handle;
}
HTCPDataComponent::~HTCPDataComponent()
{
    Finish();
}
bool HTCPDataComponent::OnRead(HBuffer *buffer)
{
    cout<<"Data on read"<<endl;
    bool bRet = false;
    ssize_t iReadLen = -1;
    char aucBuff[1024] = {0};
    while (0 < (iReadLen = recv(GetHandle(), aucBuff, sizeof(aucBuff), MSG_DONTWAIT)))
    {
        buffer->Append(aucBuff,iReadLen);
        bRet = true;
    }
    if(!bRet)
    {
        SetClosed(true);
    }
    return bRet;
}
bool HTCPDataComponent::OnWrite(const HBuffer *buffer)
{
    bool bRet = false;
    if ((0 <= GetHandle()) && (buffer->GetSize() == send(GetHandle(), buffer->GetData(), buffer->GetSize(), 0)))
    {
        bRet = true;
    }
    return bRet;
}
bool HTCPDataComponent::Initialize()
{
    return true;
}
void HTCPDataComponent::Finish()
{
    if (0 <= mHandle)
    {
        close(mHandle);
        mHandle = -1;
    }
}
