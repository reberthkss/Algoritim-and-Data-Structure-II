#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void heapify(int *vet, int n, int size) {
    int left = 2*n;
    int right = 2*n + 1;
    int max = n;
    if (left < size && (vet[left] > vet[n])) {
        max = left;
    }
    if (right < size && (vet[right] > vet[max])) {
        max = right;
    }
    if (max != n) {
        int aux = vet[n];
        vet[n] = vet[max];
        vet[max] = aux;
        heapify(vet, max, size);
    }
}
void buildMaxHeap(int *vet, int n) {
    for (int i = floor(n/2) ; i >= 0 ; i--) {
        heapify(vet, i, n);
    }
}
void heapSort(int *vet, int size) {
    buildMaxHeap(vet, size);
    for (int i = size-1 ; i >= 0 ; i--) {

        int aux = vet[0];
        vet[0] = vet[i];
        vet[i] = aux;

        heapify(vet, 0, i);
    }
}




int split(int *vector, int init, int end) {
    int i = init;
    int pivot = vector[end];
    for (int j = init ; j < end ; j++) {
        if (vector[j] <= pivot) {
            int aux = vector[j];
            vector[j] = vector[i];
            vector[i] = aux;
            i++;
        }
    }
    int aux = vector[i];
    vector[i] = vector[end];
    vector[end] = aux;
    return i;
}

void quickSort(int *vector, int init, int end) {
    if (end > init) {
        int anchor = split(vector, init, end);
        quickSort(vector, init, anchor - 1);
        quickSort(vector, anchor + 1, end);
    }
}

int main() {

    int vector[] = {22,35,2,1,13,7};
//    quickSort(vector, 0, 5);
    heapSort(vector, 6);
    for (int i = 0 ; i < 6 ; i++) {
        printf("%i ", vector[i]);
    }
    return 0;
}