#pragma once
#if defined(__APPLE__)
#define __HUOLUN_PLATFORM_APPLE__
#define __HUOLUN_KQUEUE__
#elif defined(__FreeBSD__)
#define __HUOLUN_PLATFORM_FREEBSD__
#define __HUOLUN_KQUEUE__
#elif defined(linux)
#define __HUOLUN_PLATFORM_LINUX__
#define __HUOLUN_EPOLL__
#elif defined(_WIN32)
#define __HUOLUN_PLATFORM_WIN32__
#define __HUOLUN_IOCP__
#endif
