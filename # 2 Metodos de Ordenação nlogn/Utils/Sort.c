//
// Created by Reberth on 08/12/2020.
//

#include <math.h>
#include <stdio.h>

int getPivot(int *vet, int init, int end) {
    int i = init;
    int pivot = vet[end];
    for (int j = init ; j < end ; j++) {
        if (vet[j] <= pivot) {
            int aux = vet[j];
            vet[j] = vet[i];
            vet[i] = aux;
            i++;
        }
    }
    int aux = vet[i];
    vet[i] = vet[end];
    vet[end] = aux;
    return i;
}

void quickSortInt(int *vet, int init, int end) {
    if (init < end) {
        int pivot = getPivot(vet, init, end);
        quickSortInt(vet, init, pivot-1);
        quickSortInt(vet, pivot+1, end);
    }
}


void heapify(int *vet, int i, int size) {
    int left = 2*i;
    int right = 2*i + 1;
    int max = i;
    if (left < size && (vet[left] > vet[i])) {
        max = left;
    }
    if (right < size && (vet[right] > vet[max])) {
        max = right;
    }
    if (max != i) {
        int aux = vet[i];
        vet[i] = vet[max];
        vet[max] = aux;
        heapify(vet, max, size);
    }
}



void buildMaxHeap(int *vet, int size) {
    for (int i = floor(size/2) ; i >= 0 ; i--) {
        heapify(vet, i, size);
    }
}


void heapSortInt(int *vet, int size) {
    buildMaxHeap(vet, size);
    for (int i = size-1 ; i >= 0 ; i--) {
        int aux = vet[0];
        vet[0] = vet[i];
        vet[i] = aux;
        heapify(vet, 0, i);
    }
}


void merge(int *vet, int init, int middle, int end) {
    int left[middle-init];
    int right[end-middle];
    int aux = 0;
    /* Fill Fields */
    for (int i = init ; i < middle ; i++) {
        left[aux++] = vet[i];
    }
    /* Reinitialize aux */
    aux = 0;
    /* Fill Fields */
    for (int i = middle ; i < end ; i++) {
        right[aux++] = vet[i];
    }
    int i = 0, j = 0;
    /* Get higher */
    for (int k = init ; k < end ; k++) {
        if (i >= (middle-init)) {
            vet[k] = right[j++];
        } else if (j >= (end-middle)) {
            vet[k] = left[i];
        } else if (left[i] < right[j]) {
            vet[k] = left[i++];
        } else {
            vet[k] = right[j++];
        }
    }
}

void mergeSort(int *vet, int init, int end) {
    if (end - init > 1) {
        /* Get middle */
        int middle = (int) (init + end )/2;
        /* Split */
        mergeSort(vet, init, middle);
        mergeSort(vet, middle, end);
        /* Merge */
        merge(vet, init, middle, end);
    }
}