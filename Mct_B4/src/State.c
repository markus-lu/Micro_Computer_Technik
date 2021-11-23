#include "State.h"

#define COMPARE(value) old_time->value != new_time->value

bool compare_times(struct DateTime *old_time, struct DateTime *new_time) {
    return COMPARE(seconds) ||
           COMPARE(minutes) ||
           COMPARE(hours) ||
           COMPARE(weekday) ||
           COMPARE(day) ||
           COMPARE(month) ||
           COMPARE(century) ||
           COMPARE(year);
}