#include "HuolunCore/HMessage.h"
#include "HuolunCore/HBuffer.h"
HBytesMessage::HBytesMessage()
{
    mBytes = Create<HBuffer>();
}
HBytesMessage::~HBytesMessage()
{
    if(mBytes)
    {
        mBytes->Release();
    }
}
