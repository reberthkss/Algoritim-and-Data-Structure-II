#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void swap(char *a, char *b) {
    char aux = *a;
    *a = *b;
    *b = aux;
}

void permute(char *string, char *string2, int init, int end) {
    char *aux;
    int size;
    if (string == NULL) {
        size = strlen(string2);
    } else {
        size = strlen(string);
    }
    if (string == NULL) string = "";
    aux = (char *) malloc(size * sizeof(char));
    if (init >= end) init = 0;
    if (strcmp(string, string2) != 0) {
        strcpy(aux, strcmp(string, "") == 0 ? string2 : string);
        printf("%s\n", aux);
        swap((aux + init), (aux + init + 1));
        permute(aux, string2, init + 1, end);
    }
}

int main() {
    int i;
    char *string;
    scanf("%i", &i);
    for (int j = 0 ; j < i ; j++) {
        scanf("%s", string);
        permute(NULL, string, 0, strlen(string));
        printf("\n");
    }
    return 0;
}