#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/HTCPListenerComponent.h"
#include "HuolunCore/HTCPDataComponent.h"
#include <cstring>
#ifdef __HUOLUN_PLATFORM_WIN__
#include<WS2tcpip.h>
#else
#   include <arpa/inet.h>
#   include <netinet/in.h>
#endif
HTCPListenerComponent::HTCPListenerComponent(uint16_t port , HLayerFactory *fact)
{
    if(fact)
    {
        mLayerFact = fact;
        mLayerFact->Retain();
    }
    mPort = port;
};
void HTCPListenerComponent::BindAddress(std::array<uint8_t,4> ipv4,uint16_t port)
{
    mIpv4 = ipv4;
    mPort = port;
}
HTCPListenerComponent::~HTCPListenerComponent()
{
    if(mLayerFact!=nullptr)
    {
        mLayerFact->Release();
        mLayerFact=nullptr;
    }
}
bool HTCPListenerComponent::OnRead(HBuffer *buffer)
{
    bool ret = false;
    socket_handle_t fd = -1;
    struct sockaddr_in stClientAddr;
    //TODO bind address use mip
    socklen_t lAddrLen = sizeof(stClientAddr);
    fd = accept(socket_handle_t(GetHandle()), (struct sockaddr *)&stClientAddr, &lAddrLen);
    if (0 <= fd)
    {
        //TODO ip
        auto poTcpDataChannel = Create<HTCPDataComponent>(reactor_handle_t(fd),ipv4_t{0},ntohs(stClientAddr.sin_port));
        auto layer = mLayerFact->CreateLayer(poTcpDataChannel);
        poTcpDataChannel->SetNextHandler(layer);
        mReactor->Install(poTcpDataChannel);
        ret = true;
    }
    else
    {
        perror("__FUNC__:accept:");
    }
    return ret;
}
bool HTCPListenerComponent::OnWrite(const HBuffer *buffer)
{
    return false;
}
bool HTCPListenerComponent::Initialize()
{
    bool ret = false;
    socket_handle_t iListenFd = socket_handle_t(HUOLUN_INVALID_HANDLE);
    struct sockaddr_in stServaddr;

    iListenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 <= iListenFd)
    {
        memset(&stServaddr,0,sizeof(stServaddr));
        stServaddr.sin_family = AF_INET;
        stServaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        stServaddr.sin_port = htons(mPort);

        int opt = 1;
        setsockopt(iListenFd, SOL_SOCKET, SO_REUSEADDR,  (char*)&opt, sizeof(opt));
        if (0 == bind(iListenFd, (struct sockaddr *)&stServaddr, sizeof(stServaddr)))
        {
            if (0 == listen(iListenFd, 10))
            {
                ret = true;
                mHandle = reactor_handle_t(iListenFd);
            }
            else
            {
                perror("__FUNC__:listen:");
            }
        }
        else
        {
            perror("__FUNC__:bind:");
        }
    }
    else
    {
        perror("__FUNC__:socket:");
    }
	return ret;
}
void HTCPListenerComponent::Finish()
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
