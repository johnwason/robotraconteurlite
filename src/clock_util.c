#include "robotraconteurlite/clock.h"
#include "robotraconteurlite/util.h"

#define FAILED ROBOTRACONTEURLITE_FAILED

robotraconteurlite_status robotraconteurlite_clock_timeout_from_now(struct robotraconteurlite_clock* clock,
                                                                    robotraconteurlite_i32 timeout_us,
                                                                    robotraconteurlite_timespec* timeout_out)
{
    robotraconteurlite_timespec now = 0;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_clock_gettime(clock, &now);
    if (FAILED(rv))
    {
        return rv;
    }

    *timeout_out = now + (timeout_us);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}
