#include "HuolunCore/HPlatform.h"
#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/HTCPDataComponent.h"
#ifdef __HUOLUN_PLATFORM_WIN__
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#endif
HTCPDataComponent::HTCPDataComponent(reactor_handle_t handle,std::array<uint8_t,4> ip,uint16_t port)
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
    bool bRet = false;
    size_t iReadLen = -1;
    char aucBuff[1024] = {0};
    while (0 < (iReadLen = recv(socket_handle_t(GetHandle()), aucBuff, sizeof(aucBuff),0)))
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
    if ((0 <= GetHandle()) && (buffer->GetSize() == send(socket_handle_t(GetHandle()), buffer->GetData(), buffer->GetSize(), 0)))
    {
        bRet = true;
    }
    return bRet;
}
bool HTCPDataComponent::Initialize()
{
#ifdef UNIX  
    int on=1;  
    if (ioctlsocket(id, FIONBIO, (char *)&on) < 0)  
#endif  
              
#ifdef WIN32  
    unsigned long on_windows=1;  
    if (ioctlsocket(socket_handle_t(mHandle), FIONBIO, &on_windows) < 0)  
#endif  
    return true;
}
void HTCPDataComponent::Finish()
{
    if (0 <= mHandle)
    {
#ifdef __HUOLUN_PLATFORM_WIN__
        closesocket(socket_handle_t(mHandle));
#else
        close(mHandle);
#endif
        mHandle = HUOLUN_INVALID_HANDLE;
    }
}
