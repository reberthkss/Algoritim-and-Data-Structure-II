#include <stdlib.h>
#include <stdio.h>
#include "Utils/Performance.h"
#include "Utils/Sort.h"
#include <string.h>

#define HUNDRED_THOUSAND 100000

void runMockSortTest(char *type) {
    int elements[HUNDRED_THOUSAND];
    clock_t start = startTiming();
    for (int i = 0; i < HUNDRED_THOUSAND; i++) {
        elements[i] = abs(rand() % 10);
    }
    if(strcmp(type, "quick") == 0) {
        quickSortInt(elements, 0, HUNDRED_THOUSAND);
        printf("Quick sort ");
    } else if (strcmp(type, "merge") == 0){
        printf("Merge sort ");
        mergeSort(elements, 0, HUNDRED_THOUSAND);
    }
    endTiming(start);

}

int main() {
    runMockSortTest("quick");
    runMockSortTest("merge");
}