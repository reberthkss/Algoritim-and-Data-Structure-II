//
// Created by Reberth on 21/12/2020.
//


#include "Performance.h"
#include <time.h>
#include <stdio.h>

clock_t startTiming() {
    return clock();
}

void endTiming(clock_t startTime) {
    double timingLapsed = ((double)(clock() - startTime))/CLOCKS_PER_SEC;
    printf("Time lapsed => %lf seconds\n", timingLapsed);
}

