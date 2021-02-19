#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 15
#include <time.h>

clock_t startTiming() {
    return clock();
}

void endTiming(clock_t startTime) {
    double timingLapsed = ((double)(clock() - startTime))/CLOCKS_PER_SEC;
    printf("Time lapsed => %lf seconds\n", timingLapsed);
}


typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} *PNode;

PNode getNode() {
    PNode node = NULL;
    return (PNode) malloc(sizeof(struct Node));
}

PNode getMostHigherLeaf(PNode node, PNode prev, PNode *leaf) {
    if (node == NULL) {
        return NULL;
    } else if (node->right == NULL && node->left == NULL) {
        *leaf = node;
        return prev;
    } else if (node->right != NULL && node->left == NULL) {
        return getMostHigherLeaf(node->right, node, leaf);
    } else if (node->left != NULL && node->right == NULL) {
        return getMostHigherLeaf(node->left, node, leaf);
    }
}

void freeBinary(PNode root) {
    if (root != NULL) {
        freeBinary(root->left);
        freeBinary(root->right);
        free(root);
    }
}

PNode removeNode(PNode root, PNode prev, int data) {
    PNode newRoot, leaf, prevMostHigherLeaf;
    if (root == NULL) {
        return NULL;
    } else if (root->data == data) {
        if (prev == NULL) {
            /* is root */
            if (root->left == NULL && root->right == NULL) {
                free(root);
                return NULL;
            } else if (root->right != NULL && root->left == NULL) {
                newRoot = root->right;
                free(root);
                return newRoot;
            } else if (root->left != NULL && root->right == NULL) {
                newRoot = root->left;
                free(root);
                return newRoot;
            } else {
                prevMostHigherLeaf = getMostHigherLeaf(root, NULL, &leaf);
                if (prevMostHigherLeaf == NULL) {
                    freeBinary(root);
                    return NULL;  /* ERROR */
                } else {
                    if (prevMostHigherLeaf->left == leaf) {
                        prevMostHigherLeaf->left = NULL;
                    } else if (prevMostHigherLeaf->right == leaf) {
                        prevMostHigherLeaf->right = NULL;
                    }
                    leaf->left = root->left;
                    leaf->right = root->right;
                    free(root);
                    return leaf;
                }
            }
        } else {
            prevMostHigherLeaf = getMostHigherLeaf(root->right, NULL, &leaf);

            if (root->left != leaf) {
                leaf->left = root->left;
            } else {
                leaf->left = NULL;
            }

            if (root->right != leaf) {
                leaf->right = root->right;
            } else {
                leaf->right = NULL;
            }

            if (prevMostHigherLeaf != root && prevMostHigherLeaf->right == leaf) {
                prevMostHigherLeaf->right = leaf;
            } else if (prevMostHigherLeaf != root && prevMostHigherLeaf->left == leaf) {
                prevMostHigherLeaf->left = leaf;
            } else {
                if (prev->left == root) {
                    prev->left = leaf;
                } else {
                    prev->right = leaf;
                }
            }
            free(root);
            return leaf;
        }
    } else if (root->data > data) {
        return removeNode(root->right, root, data);
    } else if (root->data < data) {
        return removeNode(root->left, root, data);
    } else {
        return root;
    }
}

void insertionSort(int vet[], int size) {
    for (int i = 0 ; i < size ; i++) {
        for (int j = 0 ; j < size ; j++) {
            if (vet[i] < vet[j]) {
                int aux = vet[i];
                vet[i] = vet[j];
                vet[j] = aux;
            }
        }
    }
}

void printInfixBinaryTree(PNode root) {
    if (root != NULL) {
        printInfixBinaryTree(root->left);
        printf("%i ", root->data);
        printInfixBinaryTree(root->right);
    }
}

PNode createBinaryTree(PNode root, int value) {
    if (root == NULL) {
        PNode node = getNode();
        node->right = NULL;
        node->left = NULL;
        node->data = value;
        return node;
    } else if (root->data > value) {
        root->left = createBinaryTree(root->left, value);
    } else if (root->data < value) {
        root->right = createBinaryTree(root->right, value);
    }
    return root;
}

int getNumber(PNode root, int number) {
    if (root == NULL) {
        return 0;
    } else if (root->data > number) {
        return getNumber(root->left, number);
    } else if (root->data < number) {
        return getNumber(root->right, number);
    } else {
//        printf("Numero encontrado!\n");
        return 1;
    }
    return 0;
}

int getNumberVect(int *vet, int size, int number) {
    for (int i = 0 ; i < size ; i++) {
        if (vet[i] == number) {
            printf("%i found!!\n", number);
            return 1;
        }
    }
    printf("%i not found!\n");
    return 0;
}

void searchBinaryTree(PNode root, int number) {
    time_t start = startTiming();
    if (getNumber(root, number)) {
        printf("%i found!\n", number);
    } else {
        printf(" %i not found !\n", number);
    }
    endTiming(start);
}

void searchVector(int *vet, int number) {
    time_t start = startTiming();
    if (getNumberVect(vet, ARRAY_SIZE, number)) {
//        printf("Ok\n");
    } else {
//        printf("N ok!\n");
    }
    endTiming(start);
}

int getBinaryHeight(PNode root, int height) {
   if (root == NULL) {
       return height;
   } else {
       int heightLeft = getBinaryHeight(root->left, height+1);
       int heightRight = getBinaryHeight(root->right, height+1);
       if (heightLeft > heightRight) {
           return heightLeft;
       } else {
           return heightRight;
       }
   }
}

void printLeafs(PNode root) {
    if (root == NULL) {
        return;
    } else if (root->left == NULL && root->right == NULL) {
        printf("%i ", root->data);
    } else if (root->left != NULL && root->right != NULL) {
        printLeafs(root->left);
        printLeafs(root->right);
    } else if (root->left != NULL) {
        printLeafs(root->left);
    } else {
        printLeafs(root->right);
    }
}
int main() {
    int vet[ARRAY_SIZE];
    time_t start = NULL;
    PNode root = NULL;

    /* Inserir vetor! */
    /*start = startTiming();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        vet[i] = rand() % ARRAY_SIZE;
    }
    endTiming(start);*/

//    /* Ordenar !*/
//    insertionSort(vet, ARRAY_SIZE);

    /* Inserir arvore! */
    /*start = startTiming();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        root = createBinaryTree(root, rand() % ARRAY_SIZE);
    }
    endTiming(start);*/

    /*printf("Search binary => \n");
    searchBinaryTree(root, 50);
    searchBinaryTree(root, 5000);
    printf("end binary!\n");*/

    /*printf("Search vector => \n");
    searchVector(vet, 50);
    searchVector(vet, 5000);
    printf("end vector!\n");*/

    /*printf("Binary height LEFT => %i\n", getBinaryHeight(root->left, 0));
    printf("Binary height RIGHT => %i\n", getBinaryHeight(root->right, 0));

    printf("Leafs => ");
    printLeafs(root);
    printInfixBinaryTree(root);*/

    root = createBinaryTree(root, 7);
    root = createBinaryTree(root, 5);
    root = createBinaryTree(root, 6);
    root = createBinaryTree(root, 4);
    root = createBinaryTree(root, 3);
    root = createBinaryTree(root, 2);
    root = createBinaryTree(root, 9);
    root = createBinaryTree(root, 10);
    root = createBinaryTree(root, 8);
    printInfixBinaryTree(root);

}