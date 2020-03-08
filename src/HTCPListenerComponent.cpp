#include "HuolunCore/Reactor/HReactor.h"
#include "HuolunCore/HTCPListenerComponent.h"
#include "HuolunCore/HTCPDataComponent.h"
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
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
    int fd = -1;
    struct sockaddr_in stClientAddr;
    //TODO bind address use mip
    socklen_t lAddrLen = sizeof(stClientAddr);
    fd = accept(GetHandle(), (struct sockaddr *)&stClientAddr, &lAddrLen);
    if (0 <= fd)
    {
        //TODO ip
        auto poTcpDataChannel = Create<HTCPDataComponent>(fd,ipv4_t{0},ntohs(stClientAddr.sin_port));
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
    int iListenFd = -1;
    struct sockaddr_in stServaddr;

    iListenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 <= iListenFd)
    {
        memset(&stServaddr,0,sizeof(stServaddr));
        stServaddr.sin_family = AF_INET;
        stServaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        stServaddr.sin_port = htons(mPort);

        int opt = 1;
        setsockopt(iListenFd, SOL_SOCKET, SO_REUSEADDR,  (void *)&opt, sizeof(opt));
        if (0 == bind(iListenFd, (struct sockaddr *)&stServaddr, sizeof(stServaddr)))
        {
            if (0 == listen(iListenFd, 10))
            {
                ret = true;
                mHandle = iListenFd;
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
        close(mHandle);
        mHandle = -1;
    }
}
