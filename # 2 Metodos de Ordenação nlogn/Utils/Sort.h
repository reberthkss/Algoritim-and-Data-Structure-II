//
// Created by Reberth on 08/12/2020.
//

#ifndef UNTITLED1_SORT_H
#define UNTITLED1_SORT_H

/* Quick Sort */
void quickSortInt(int *vet, int init, int end);
int getPivot(int *vet, int init, int end);

/* Heap Sort */

void heapSortInt(int *vet, int size);
void buildMaxHeap(int *vet, int size);
void heapify(int *vet, int i, int size);



/* Merge Sort */

void mergeSort(int *vet, int init, int end);
void merge(int *vet, int init, int middle, int end);
#endif //UNTITLED1_SORT_H
