#include "robotraconteurlite/util.h"
#include "robotraconteurlite/array.h"

#define FAILED ROBOTRACONTEURLITE_FAILED

robotraconteurlite_status robotraconteurlite_util_match_string_in_list(
    const struct robotraconteurlite_const_string* list, const struct robotraconteurlite_const_string* value)
{
    robotraconteurlite_size_t i = 0;
    robotraconteurlite_size_t k = 0;
    robotraconteurlite_status rv = -1;
    for (i = 0; i < list->len; i++)
    {
        if (list->data[i] == ((char)';'))
        {
            if ((i - k) > 0U)
            {
                struct robotraconteurlite_const_string t2;
                t2.data = &list->data[k];
                t2.len = (i - k);

                rv = (robotraconteurlite_status)((robotraconteurlite_string_cmp(&t2, value) == 0) ? 1 : 0);
                if (rv == 1)
                {
                    return rv;
                }
                if (FAILED(rv))
                {
                    return rv;
                }
            }
            k = i + 1U;
        }
    }

    if (k < list->len)
    {
        struct robotraconteurlite_const_string t2;
        t2.data = &list->data[k];
        t2.len = (list->len - k);
        return (robotraconteurlite_string_cmp(&t2, value) == 0) ? 1 : 0;
    }

    return 0U;
}
