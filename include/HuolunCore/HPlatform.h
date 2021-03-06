#pragma once
#if defined(__APPLE__)
#define __HUOLUN_PLATFORM_APPLE__
#define __HUOLUN_KQUEUE__
#define __HUOLUN_SELECT__
#define __HUOLUN_POLL__
#define __HUOLUN_SUPPORT_UNISTD__
#elif defined(__FreeBSD__)
#define __HUOLUN_PLATFORM_FREEBSD__
#define __HUOLUN_KQUEUE__
#define __HUOLUN_SELECT__
#define __HUOLUN_POLL__
#define __HUOLUN_SUPPORT_UNISTD__
#elif defined(linux)
#define __HUOLUN_PLATFORM_LINUX__
#define __HUOLUN_EPOLL__
#define __HUOLUN_SELECT__
#define __HUOLUN_POLL__
#define __HUOLUN_SUPPORT_UNISTD__
#elif defined(_WIN32)
#define __HUOLUN_PLATFORM_WIN__
#define __HUOLUN_PLATFORM_WIN32__
#elif defined(_WIN64)
#define __HUOLUN_PLATFORM_WIN__
#define __HUOLUN_PLATFORM_WIN64__
#endif
#ifdef __HUOLUN_PLATFORM_WIN__
//#define __HUOLUN_IOCP__
#define __HUOLUN_SELECT__
#endif // __HUOLUN_PLATFORM_WIN__

