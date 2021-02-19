#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define ELEMENTS 10000
typedef struct Node {
    int value, height;
    struct Node *left;
    struct Node *right;
} *PNode;

/* UTILS */


clock_t startTiming() {
    return clock();
}

void endTiming(clock_t startTime) {
    double timingLapsed = ((double) (clock() - startTime)) / CLOCKS_PER_SEC;
    printf("Time lapsed => %lf seconds\n", timingLapsed);
}


int getHeight(PNode root, int currentHeight) {
    if (root == NULL) {
        return currentHeight;
    } else {
        int heightLeft = getHeight(root->left, currentHeight + 1);
        int heightRight = getHeight(root->right, currentHeight + 1);
        if (heightLeft > heightRight) {
            return heightLeft;
        } else {
            return heightRight;
        }
    }
}

int getHeightByNode(PNode root) {
    if (root == NULL) {
        return -1;
    } else {
        return root->height;
    }
}

int getHigher(int x, int y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

int balancingFactor(PNode root) {
    return fabs(getHeightByNode(root->left) - getHeightByNode(root->right));
}


/* Binary tree operations */
PNode getNode(int value) {
    PNode node = (PNode) malloc(sizeof(struct Node) * 1);
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return node;
}

void rotateRR(PNode *root) {
    /* Get right node of root son */
    PNode rightNode = (*root)->right;
    /* Right son of the root receive the left node of the rightNode */
    (*root)->right = rightNode->left;
    /* Left side of rightNode appoint to root */
    rightNode->left = (*root);
    /* Get new height to root */
    (*root)->height = getHigher(getHeightByNode((*root)->left), getHeightByNode((*root)->right)) + 1;
    /* Get new height to rightNode (new Root) */
    rightNode->height = getHigher((*root)->height, getHeightByNode(rightNode)) + 1;
    /* Change roots */
    *root = rightNode;
}

void rotationLL(PNode *root) {
    /* Get the left son of the root */
    PNode leftNode = (*root)->left;
    /* Left son of the root receive right sun of leftNode*/
    (*root)->left = leftNode->right;
    /* Right son of the left sun of root receive root */
    leftNode->right = (*root);
    /* Get new height to root */
    (*root)->height = getHigher(getHeightByNode((*root)->left), getHeightByNode((*root)->right)) + 1;
    /* Get new height to leftNode */
    leftNode->height = getHigher(getHeightByNode(leftNode->left), (*root)->height) + 1;
    /* Change roots */
    *root = leftNode;
}

void rotationLR(PNode *root) {
    rotateRR(&(*root)->left);
    rotationLL(root);
}

void rotationRL(PNode *root) {
    rotationLL(&(*root)->right);
    rotateRR(root);
}

int getNumber(PNode root, int number) {
    if (root == NULL) {
        return 0;
    } else if (root->value > number) {
        return getNumber(root->left, number);
    } else if (root->value < number) {
        return getNumber(root->right, number);
    } else {
//        printf("Numero encontrado!\n");
        return 1;
    }
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

PNode getLowerNode(PNode root) {
    if (root->left == NULL) {
        return root;
    } else {
        return getLowerNode(root->left);
    }
}

int insert(PNode *root, int value) {
    PNode node = NULL;
    int res;
    if (*root == NULL) {
        node = getNode(value);
        *root = node;
        return 1;
    } else if ((*root)->value < value) {
        if ((res = insert(&(*root)->right, value)) == 1) {
            if (balancingFactor(*root) >= 2) {
                if ((*root)->right->value < value) {
                    rotateRR(root);
                } else {
                    rotationRL(root);
                }
            }
        }
    } else if ((*root)->value > value) {
        if ((res = insert(&(*root)->left, value)) == 1) {
            if (balancingFactor(*root) >= 2) {
                if ((*root)->left->value > value) {
                    rotationLL(root);
                } else {
                    rotationLR(root);
                }
            }
        }
    } else {
//        printf("Both are equal!\n");
        return 0; /* ERROR */
    }
    (*root)->height = getHigher(getHeightByNode((*root)->left),
                                getHeightByNode((*root)->right)) + 1;
    return res; /* RESPONSE */
}

int removeAVL(PNode *root, int value) {
    int res;
    if (*root == NULL) {
        printf("Value doesn't exist!");
        return 0;
    } else if ((*root)->value > value) {
        if ((res = removeAVL(&(*root)->left, value)) == 1) {
            if (balancingFactor(*root) > 1) {
                if (getHeightByNode((*root)->right->left) <= getHeightByNode((*root)->right->right)) {
                    rotateRR(root);
                } else {
                    rotationRL(root);
                }
            }
        }
    } else if ((*root)->value < value) {
        if ((res = removeAVL(&(*root)->right, value)) == 1) {
            if (balancingFactor(*root) > 1) {
                if (getHeightByNode((*root)->left->left) >= getHeightByNode((*root)->left->right)) {
                    rotationLL(root);
                } else {
                    rotationLR(root);
                }
            }
        }
    } else /* (*root)->value == value */ {
        if ((*root)->left == NULL || (*root)->right == NULL) {
            PNode aux = NULL;
            if ((*root)->left != NULL) {
                aux = (*root)->left;
            } else {
                aux = (*root)->right;
            }
            free(*root);
            *root = aux;
        } else {
            PNode lowerNode = getLowerNode((*root)->right);
            (*root)->value = lowerNode->value;
            removeAVL(&(*root)->right, (*root)->value);
            if (balancingFactor(*root) > 1) {
                if (getHeightByNode((*root)->left->left) >= getHeightByNode((*root)->left->right)) {
                    rotationLL(root);
                } else {
                    rotationLR(root);
                }
            }
        }
        return 1;
    }
    return res;
}

int main(void) {
    PNode root = NULL;
    time_t start = startTiming();
    for (int i = 0; i < ELEMENTS;) {
        if (insert(&root, rand() % (i + 1))) {
//            printf("success!\n");
            i++;
        } else {
//            printf("Failure!\n");
        }
    }
    endTiming(start);

    removeAVL(&root, 362);
    searchBinaryTree(root, 50);
    searchBinaryTree(root, 5000);
    return 0;
}
