#include "timer.h"
#include <sys/time.h>
#include <stddef.h> 

static struct timeval start;
static int delay = 0;

void timerInit(int valueMilliSec) {
    delay = valueMilliSec;
    gettimeofday(&start, NULL);
}

void timerDestroy() {
    delay = 0;
}

int timerTimeOver() {
    struct timeval now;
    gettimeofday(&now, NULL);
    int elapsed = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000;
    if (elapsed > delay) {
        gettimeofday(&start, NULL);
        return 1;
    }
    return 0;
}
