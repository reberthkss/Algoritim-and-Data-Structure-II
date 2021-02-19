#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define true 1;
#define false 0;
#define none 0;



clock_t startTiming() {
    return clock();
}

void endTiming(clock_t startTime) {
    double timingLapsed = ((double)(clock() - startTime))/CLOCKS_PER_SEC;
    printf("Time lapsed => %lf seconds\n", timingLapsed);
}

typedef struct BBinary {
    int elements, order, * values, isLeaf;
    struct BBinary ** pointers;
} * PBBinary;


void printfBBinary(PBBinary root) {
    if (root == NULL) return;
    if (root->isLeaf) {
        for (int i = 0 ; i < root->elements ; i++) {
            printf("%i ", root->values[i]);
        }
    }
    else {
        for (int i = 0; i <= root->elements ; i++) {
            printfBBinary(root->pointers[i]);
            if (root->elements > i) printf("%i ", root->values[i]);
        }
    }
}

PBBinary generateBBinary(int order) {
    PBBinary root = (PBBinary) malloc(sizeof(struct BBinary));
    root->order = order;
    root->elements = none;
    root->isLeaf = true;
    root->pointers = (PBBinary *) malloc(sizeof(PBBinary) * ((2 * order)));
    root->values = (int*) malloc(sizeof(int)*((order*2) - 1));
    for (int i = 0 ; i < 2*order ; i++) root->values[i] = 0;
    for (int i = 0 ; i <= (2*order) ; i++) root->pointers[i] = NULL;
    return root;
}

PBBinary getBBinaryNode(int order) {
    PBBinary node = (PBBinary) malloc(sizeof(struct BBinary));
    node->order = order;
    node->elements = none;
    node->pointers = (PBBinary *) malloc(sizeof(PBBinary) * order*2);
    node->values = (int *) malloc(sizeof(int) * (2*order - 1));
    node->isLeaf = true;
    for (int i = 0 ; i < order*2 ; i++) node->values[i] = 0;
    for (int i = 0 ; i <= order*2 ; i++) node->pointers[i] = NULL;
    return node;
}


void splitPage(PBBinary father, int position, PBBinary pageToSplit) {
    int order = pageToSplit->order;
    PBBinary newBrother = getBBinaryNode(order);
    /* Save values from the middle to the end of pageToSplit in newBrother root */
    for (int i = 0 ; i < order-1; i++) { /* 0, 1, 2
 *                                         1, 2, 3  */
                                     /* 0, 1, 2, 3, 4, 5, 6
*                                      1, 2, 3, 4, 5, 6, 7  */
        newBrother->values[i] = pageToSplit->values[i+order];
    }
    newBrother->elements = order-1;
    /*MOVING POINTERS FROM PAGE TO SPLIT!*/
    /* Save pointers from the middle to the end of pageToSplit to newBrother page root */
    if (!pageToSplit->isLeaf) {
        for (int i = 0 ; i < order ; i++) {
            /* 0,  1,  2,  3 */
            /* P1, P2, P3, P4*/
            newBrother->pointers[i] = pageToSplit->pointers[i+order];
        }
    }
    /* NEW BROTHER! */
    pageToSplit->elements = order-1;
    /* ADJUSTING FATHER POINTERS! */
    for (int i = father->elements ; i > position ; i--) {
        father->pointers[i+1] = father->pointers[i];
    }
    father->pointers[position+1] = newBrother;
    /* ADJUSTING VALUES! */
    for (int i = father->elements ; i >= position ; i--) {
        father->values[i+1] = father->values[i];
    }
    father->values[position] = pageToSplit->values[order-1];
    father->elements++;
}

void insertNonFull(PBBinary * root, int value) {
    int position, order;
    order = (*root)->order;
    position = 0;
    PBBinary node = *root;

    while (node->elements > position && node->values[position] < value) position++;

    for (int i = 0 ; i < node->elements ; i++) {
        if (node->pointers[position] != NULL ) {
            node->isLeaf = false;
            break;
        }
    }

    if (node->isLeaf) {
        for (int i = node->elements ; i > position ; i--) {
            node->values[i] = node->values[i-1];
        }
        node->values[position] = value;
        node->elements++;
    }
    else {
        if (node->pointers[position]->elements >= (2*order-1)) {
            splitPage(*root, position, node->pointers[position]);
            insertNonFull(root, value);
        }
        else {
            insertNonFull(&(node->pointers[position]), value);
        }
    }
}



void insertInBBinary(PBBinary *root, char * string, int value) {
    printf("string => %s value => %i\n", string, value);/* => UNCOMMENT TO GO ALONG STEP BY STEP OF INSERTION*/
    int order;
    PBBinary node = *root, newNode = NULL;
    order = node->order;
    if (node->elements == (2*order) - 1) {
        newNode = getBBinaryNode(order);
        newNode->isLeaf = false;
        newNode->pointers[0] = node;
        splitPage(newNode, 0, node);
        insertNonFull(&newNode, value);
        *root = newNode;
    }
    else {
        insertNonFull(root, value);
    }
}


int getSize(char *string) {
    int size;
    for (size = 0 ; string[size] != '\0' ; size++);
    return size;
}

int getStringAsInt(char *string, int size) {
    int sum = 0;
    for (int i = 0 ; i < size ; i++) {
        sum += (int) string[i];
    }
    return sum;
}



void processFile(PBBinary * root, FILE *file) {
    char string[100];
    int stringIndex = 0;
    while (!feof(file)) {
        char value = fgetc(file);
        if (value == '\n') {
            insertInBBinary(root, string, getStringAsInt(string, getSize(string)));
            stringIndex = 0;
        }
        else {
            string[stringIndex++] = value;
            string[stringIndex] = '\0';
        }
    }
    fclose(file);
}



int searchForFile(PBBinary root, int value) {
    int position = 0, res;
    if (root == NULL) return 0;
    while (root->elements > position && root->values[position] < value) position++;

    if (root->elements > position && root->values[position] == value) {
        return 1;
    }

    if (root->isLeaf) {
        return 0;
    }
    else {
        return searchForFile(root->pointers[position], value);
    }
}

void printMenu() {
    printf("--------------------------------------------\n");
    printf("                    MENU                    \n");
    printf("1 - Inserir uma imagem                      \n");
    printf("2 - Inserir todas as imagens do repostorio  \n");
    printf("3 - Abrir imagem por categoria              \n");
    printf("4 - Excluir categoria                       \n");
    printf("5 - Imprimir árvore                         \n");
    printf("6 - Sair                                    \n");
    printf("Escolha uma opcao: ");
}

void freeBBinary(PBBinary *root) {
    if ((*root) == NULL) return;
    if ((*root)->isLeaf) {
        free(*root);
        *root = NULL;
    }
    else {
        for (int i = 0 ; i <= (*root)->elements ; i++) {
            freeBBinary(&(*root)->pointers[i]);
        }
        free(*root);
        *root = NULL;
    }
}

int insertAll(PBBinary *root) {
    FILE *file = NULL;
    file = fopen("data.txt", "a+");

    printf("processing file...\n");
    processFile(root, file);

    if (root != NULL) {
        printf("Successful generated B Binary!\n");
        return 1;
    }
    else {
        return 0;
    }
}

int searchByRotule(PBBinary root) {
    char rotule[10];
    printf("Insert the rotule for search: ");
    scanf("%s", rotule);
    printf("Searching for %s ...\n", rotule);
    for (int i = 1 ; i <= 5 ; i++) {
        int imgValue = getStringAsInt(rotule, getSize(rotule))+48+i;
        char numberAsString[100] = {(char) (48+i), '\0'};
        char rotuleCopy[100];
        /* foto */
        /* fotos\\fotos1.jpg*/
        /* fotos\\fotos2.jpg*/
        /* fotos\\fotos3.jpg*/
        strcpy(rotuleCopy, "fotos\\");
        strcat(rotuleCopy, rotule);
        strcat(rotuleCopy, numberAsString);
        strcat(rotuleCopy, ".jpg");

        if (searchForFile(root, imgValue)) {
            system(rotuleCopy);
        }
        else {
            return 0;
        }
    }
    return 1;
}

void insertSingleImage(PBBinary *root) {
    char * image;
    printf("Type image name: ");
    scanf("%s", image);
    insertInBBinary(root, image,  getStringAsInt(image, getSize(image)));
}

PBBinary * takeGreaterValueFromLeftChild(PBBinary * root) {
    int pos = (*root)->elements-1;
    if ((*root)->isLeaf) {
        return root;
    }
    else {
        if ((*root)->pointers[pos+1] != NULL) pos++;
        return takeGreaterValueFromLeftChild(&((*root)->pointers[pos]));
    }
}

PBBinary  *takeMinorValueFromRightChild(PBBinary * root) {
    if ((*root)->isLeaf) {
        return root;
    }
    else {
        PBBinary *minorPage = NULL;
        for (int i = 0 ; i <= (*root)->elements ; i++) {
            if ((*root)->pointers[i] != NULL) {
                minorPage = &(*root)->pointers[i];
                break;
            }
        }
        if ((*minorPage) == NULL) {
            return root;
        }
        else {
            return takeMinorValueFromRightChild(minorPage);
        }
    }
}

void mergeBrothers(PBBinary root, int position, PBBinary * leftChild, PBBinary * rightChild) {
    int value = root->values[position+1];

    for (int i = 0 ; i < (*rightChild)->elements ; i++) {
        (*leftChild)->values[(*leftChild)->elements++] = (*rightChild)->values[i];
    }

    for (int i = 0 ; i <= (*rightChild)->elements ; i++) {
        (*leftChild)->pointers[position+i] = (*rightChild)->pointers[i];
    }

    for (int i = position ; i < root->elements-2 ; i++) {
        root->values[i+1] = root->values[i+2];
    }

    for (int i = position ; i <= root->elements-2 ; i++) {
        root->pointers[i+1] = root->pointers[i+2];
    }

    root->elements--;


    for (int i = (*rightChild)->elements ; i > 0 ; i--) {
        (*rightChild)->values[i] = (*rightChild)->values[i-1];
    }
    (*rightChild)->values[0] = value;
    (*rightChild)->elements++;
    freeBBinary(rightChild);
}

int removeValueFromBBinary(PBBinary * root, int value) {
    int position = 0;
    if ((*root) == NULL) return 0;
    while ((*root)->elements > position && (*root)->values[position] < value) position++;

    if ((*root)->elements > position && (*root)->values[position] == value) {
        if ((*root)->isLeaf) {
            for (int i = position ; i <= (*root)->elements-1 ; i++) {
                (*root)->values[i] = (*root)->values[i+1];
            }

            for (int i = position ; i <= (*root)->elements ; i++) {
                (*root)->pointers[i] = (*root)->pointers[i+1];
            }
            (*root)->elements--;

            (*root)->isLeaf = true;
            for (int i = 0 ; i <= (*root)->elements ; i++) {
                if ((*root)->pointers[i] != NULL) {
                    (*root)->isLeaf = false;
                    break;
                }
            }
            return 1;
        }
        else {
            /* Check if:
             *  - Current key have child on the left and right
             *  - If is true, check which child have more elements
             *  - Select the element that have more than t-1 children
             *  - Else take the minor value from the right child then merge both childs
             *
             *  - If there is a single child, check if the child have at least t elements
             *  - If is true, take the greater key of the child
             *  - Else move up the key and the pointers
             * */
            if ((*root)->pointers[position] != NULL && (*root)->pointers[position + 1] != NULL) {
               /*
                *  - If is true, check which child have more elements
                *  - Select the element that have more than t-1 children
                *  - Else take the minor value from the right child then merge both childs
                  */
               PBBinary *pageWithTheGreatestKeyValueOfLeftChild = takeGreaterValueFromLeftChild(&((*root)->pointers[position]));
               PBBinary *pageWithTheGreatestKeyValueOfRightChild = takeMinorValueFromRightChild(&((*root)->pointers[position+1]));
               if ((*pageWithTheGreatestKeyValueOfLeftChild)->elements >= (*root)->order) {
                   (*root)->values[position] = (*pageWithTheGreatestKeyValueOfLeftChild)->values[(*pageWithTheGreatestKeyValueOfLeftChild)->elements-1];
                   (*pageWithTheGreatestKeyValueOfLeftChild)->elements--;
                   if ((*pageWithTheGreatestKeyValueOfLeftChild)->elements < (*root)->order-1) {
                       for (int i = 0 ; i < (*pageWithTheGreatestKeyValueOfLeftChild)->elements ; i++) {
                           insertInBBinary(root, "", value);
                       }
                       freeBBinary(pageWithTheGreatestKeyValueOfLeftChild);
                   }
               }
               else if ((*pageWithTheGreatestKeyValueOfRightChild)->elements >= (*root)->order) {
                   (*root)->values[position] = (*pageWithTheGreatestKeyValueOfRightChild)->values[0];
                   for (int i = 0; i < (*pageWithTheGreatestKeyValueOfRightChild)->elements - 1; i++) {
                       (*pageWithTheGreatestKeyValueOfRightChild)->values[i] = (*pageWithTheGreatestKeyValueOfRightChild)->values[
                               i + 1];
                   }
                   (*pageWithTheGreatestKeyValueOfRightChild)->elements--;
                   if ((*pageWithTheGreatestKeyValueOfRightChild)->elements < (*root)->order - 1) {
                       for (int i = 0; i < (*pageWithTheGreatestKeyValueOfRightChild)->elements; i++) {
                           insertInBBinary(root, "", (*pageWithTheGreatestKeyValueOfRightChild)->values[i]);
                       }
                       freeBBinary(pageWithTheGreatestKeyValueOfRightChild);
                   }
               }
               else {
                       /* take the minor value from the right child then merge both children */
                       (*root)->values[position] = (*pageWithTheGreatestKeyValueOfRightChild)->values[0];

                       for (int i = 0; i < (*pageWithTheGreatestKeyValueOfRightChild)->elements - 1; i++) {
                           (*pageWithTheGreatestKeyValueOfRightChild)->values[i] = (*pageWithTheGreatestKeyValueOfRightChild)->values[
                                   i + 1];
                       }
                       (*pageWithTheGreatestKeyValueOfRightChild)->elements--;
                       if ((*pageWithTheGreatestKeyValueOfRightChild)->elements < (*root)->order - 1) {
                           for (int i = 0; i < (*pageWithTheGreatestKeyValueOfRightChild)->elements; i++) {
                               insertInBBinary(root, "", (*pageWithTheGreatestKeyValueOfRightChild)->values[i]);
                           }
                           freeBBinary(pageWithTheGreatestKeyValueOfRightChild);
                       }
                   }
               return 1;
            }
            else if ((*root)->pointers[position] != NULL) {
                /*
                    * - If there is a single child, check if the child have at least t elements
                    *  - If is true, take the greater key of the child
                    *  - Else move up the key and the pointers
                    *  */
                PBBinary * greatestPageFromTheLeftChild = takeGreaterValueFromLeftChild(&((*root)->pointers[position]));
                /* take the greater key of the child */
                (*root)->values[position] = (*greatestPageFromTheLeftChild)->values[(*greatestPageFromTheLeftChild)->elements-1];
                (*greatestPageFromTheLeftChild)->elements--;
                if ((*greatestPageFromTheLeftChild)->elements < (*root)->order-1) {
                    for (int i = 0 ; i < (*greatestPageFromTheLeftChild)->elements ; i++) {
                        insertInBBinary(root, "", (*greatestPageFromTheLeftChild)->values[i]);
                    }
                }
                freeBBinary(greatestPageFromTheLeftChild);
                return 1;
            }
            else {
                /*
                    * - If there is a single child, check if the child have at least t elements
                    *  - If is true, take the greater key of the child
                    *  - Else move up the key and the pointers
                    *  */
                PBBinary * greatestPageFromTheRightChild = takeMinorValueFromRightChild(&((*root)->pointers[position+1]));
                (*root)->values[position] = (*greatestPageFromTheRightChild)->values[0];

                for (int i = 0 ; i < (*greatestPageFromTheRightChild)->elements-1 ; i++) {
                    (*greatestPageFromTheRightChild)->values[i] = (*greatestPageFromTheRightChild)->values[i+1];
                }

                (*greatestPageFromTheRightChild)->elements--;
                if ((*greatestPageFromTheRightChild)->elements < (*root)->order-1) {
                     /*take the greater key of the child */
                    for (int i = 0 ; i < (*greatestPageFromTheRightChild)->elements ; i++) {
                        insertInBBinary(root, "", (*greatestPageFromTheRightChild)->values[i]);
                    }

                    freeBBinary(greatestPageFromTheRightChild);
                }
                return 1;
            }
        }
    }
    else {
        if ((*root)->isLeaf) {
            return 0; /* Value not found */
        }
        else {
            int res = removeValueFromBBinary(&(*root)->pointers[position], value);
            if (res) {
                if ((*root)->pointers[position]->elements < (*root)->order-1) {
                    if ((*root)->pointers[position+1] != NULL) {
                        mergeBrothers(*root, position, &(*root)->pointers[position], &((*root)->pointers[position+1]));
                    }
                    else {
                        for (int i = 0 ; i < (*root)->pointers[position]->elements ; i++) {
                            insertInBBinary(root, "", (*root)->pointers[position]->values[i]);
                        }
                        freeBBinary(&(*root)->pointers[position]);
                    }
                }
            }
            return res;
            /* Down to next index */
        }
    }
}

int removeByRotule(PBBinary root) {
    char rotule[10];
    printf("Insert the rotule for search: ");
    scanf("%s", rotule);
    printf("Searching for %s ...\n", rotule);
    for (int i = 1 ; i <= 5 ; i++) {
        int imgValue = getStringAsInt(rotule, getSize(rotule))+48+i;
        char numberAsString[100] = {(char) (48+i), '\0'};
        char rotuleCopy[100] = {};

        /* foto => foto1*/
        strcat(rotuleCopy, rotule);
        strcat(rotuleCopy, numberAsString);

        if (searchForFile(root, imgValue)) {
//            printf("find!\n");
            int valueToRemove = getStringAsInt(rotuleCopy, getSize(rotuleCopy));
//            printf("value => %i\n", valueToRemove);
            if (removeValueFromBBinary(&root, valueToRemove)) {
//                printf("Removed!\n");
            } else {
//                printf("Not removed!\n");
            }
        } else {
            return 0;
        }
    }
    return 1;
}

int main() {
    int selectedOption, error = 0, running = 1;
    PBBinary root = generateBBinary(3);
    int alreadyAddAllFiles = 0;
    time_t  timing = NULL;

    while (running) {
        if (error) {
            printf("Opcao invalida. Tente novamente.\n");
            printf("Escolha uma opcaoo: ");
            error = 0;
        } else {
            printMenu();
        }
        scanf("%i", &selectedOption);
        switch (selectedOption) {
            case 1: {
                timing = startTiming();
                insertSingleImage(&root);
                endTiming(timing);
            }
            case 2: {
                if (alreadyAddAllFiles) {
                    printf("Ooops! It's may look like you already add the same data from repository!\n");
                } else {
                    timing = startTiming();
                    if (!insertAll(&root)) {
                        printf("There is occour an error while processing the file. Exiting...");
                        freeBBinary(&root);
                    } else {
                        alreadyAddAllFiles = 1;
                    }
                    endTiming(timing);
                }
                break;
            }
            case 3: {
                timing = startTiming();
                if (searchByRotule(root)) {
                    printf("Success on open images!\n");
                } else {
                    printf("Failure on open or images not found!\n");
                }
                endTiming(timing);
                break;
            }
            case 4: {
                timing = startTiming();
                if (removeByRotule(root)) {
                    printf("Success on remove images!\n");
                } else {
                    printf("Failure on remove or images not found!\n");
                }
                endTiming(timing);
                break;
            }
            case 5: {
                timing = startTiming();
                printf("Printing binary...\n");
                printfBBinary(root);
                printf("\n");
                endTiming(timing);
                break;
            }
            case 6: {
                freeBBinary(&root);
                running = 0;
                break;
            }
            default: {
                error = 1;
                break;
            }
        }
    }

    return 0;
}
