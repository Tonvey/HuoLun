#include "HuoLun.h"
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
    core.AddComponent();
    core.Finish();
    return 0;
}
