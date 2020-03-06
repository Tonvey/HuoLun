#include "Huolun.h"
#include <string>
#include <iostream>
using namespace std;
int main()
{
    auto buf = HObject::Create<HBuffer>();
    string s1 = "hello";
    string s2 = "world";

    cout<<__LINE__<<endl;
    cout<<"Size:"<<buf->GetSize()<<'\t'<<"Reserved size:"<<buf->GetReservedSize()<<endl;
    buf->Append(s1);
    cout<<"Size:"<<buf->GetSize()<<'\t'<<"Reserved size:"<<buf->GetReservedSize()<<endl;

    buf->Append(s2);
    cout<<"Size:"<<buf->GetSize()<<'\t'<<"Reserved size:"<<buf->GetReservedSize()<<endl;

    HBuffer buf2(*buf);
    cout<<__LINE__<<':'<<"Size:"<<buf2.GetSize()<<'\t'<<"Reserved size:"<<buf2.GetReservedSize()<<endl;
    HBuffer buf3 = buf2;
    cout<<__LINE__<<':'<<"Size:"<<buf3.GetSize()<<'\t'<<"Reserved size:"<<buf3.GetReservedSize()<<endl;
    HBuffer buf4 = std::move(buf3);
    cout<<__LINE__<<':'<<"Size:"<<buf4.GetSize()<<'\t'<<"Reserved size:"<<buf4.GetReservedSize()<<endl;
    cout<<__LINE__<<':'<<"Size:"<<buf3.GetSize()<<'\t'<<"Reserved size:"<<buf3.GetReservedSize()<<endl;


    buf->Release();
    return 0;
}
