#pragma once
typedef int handle_t;
#define HUOLUN_IO_STAT_NONE 0
#define HUOLUN_IO_STAT_IN   1
#define HUOLUN_IO_STAT_OUT  2
#define HUOLUN_IO_STAT_IN_OUT  3
#define HUOLUN_IO_STAT_ERR  4
enum EIOStat
{
    In=1,
    Out=2,
    InAndOut=3,
};
