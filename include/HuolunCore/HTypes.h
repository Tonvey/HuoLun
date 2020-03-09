#pragma once
#include "HPlatform.h"
#define HUOLUN_IO_STAT_NONE 0
#define HUOLUN_IO_STAT_IN   1
#define HUOLUN_IO_STAT_OUT  2
#define HUOLUN_IO_STAT_IN_OUT  3
#define HUOLUN_IO_STAT_ERR  4
enum EIOStat
{
    None = 0,
    In = 1,
    Out = 2,
    InAndOut = 3,
};

#ifdef __HUOLUN_PLATFORM_WIN__
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "WS2_32")
typedef HANDLE reactor_handle_t;
typedef SOCKET socket_handle_t;
#define HUOLUN_INVALID_HANDLE INVALID_HANDLE_VALUE
#else  // unix like
#include <unistd.h>
typedef int reactor_handle_t;
typedef reactor_handle_t socket_handle_t;
#define HUOLUN_INVALID_HANDLE (-1)
#endif // __HUOLUN_PLATFORM_WIN__


