
#include <mach/mach_time.h>
#include <stdio.h>

int millitime() {

    const int64_t mil = 1000 * 1000;
    static mach_timebase_info_data_t s_timebase_info;

    if (s_timebase_info.denom == 0) {
        (void) mach_timebase_info(&s_timebase_info);
    }

    return (int)((mach_absolute_time() * s_timebase_info.numer) / (mil * s_timebase_info.denom));
}
