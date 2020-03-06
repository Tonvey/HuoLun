#pragma once
#if defined(__APPLE__)||defined(__FreeBSD__)
#define __HUOLUN_KQUEUE__
#elif defined(linux)
#define __HUOLUN_EPOLL__
#elif defined(_WIN32)
#define __HUOLUN_IOCP__
#endif
