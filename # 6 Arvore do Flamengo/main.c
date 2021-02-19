#include <stdio.h>
#include <stdlib.h>

typedef enum {
    BLACK = 1,
    RED = 0,
} COLORS;

typedef struct LLRB {
    int data;
    COLORS color;
    struct LLRB * left;
    struct LLRB * right;
} * LPLLRB;

/* GENERAL RULES
 *
 * The Root always be BLACK
 * The left child of Black node always be RED
 * The number of black nodes are the same from left to root and right to root
 *
 * */

LPLLRB getNode(int data) {
    LPLLRB node = (LPLLRB) malloc(sizeof(struct LLRB));
    if (node == NULL) return NULL;
    node->color = RED;
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

LPLLRB switchColor(LPLLRB root) {
    if (root != NULL) {
        root->color = !root->color;
        if (root->left != NULL) root->left->color = !root->left->color;
        if (root->right != NULL) root->right->color = !root->right->color;
    }
    return root;
}

LPLLRB leftRotate(LPLLRB root) {
   LPLLRB nodeThatWasClimb = root->right;
   root->right = nodeThatWasClimb->left;
   nodeThatWasClimb->left = root;
   nodeThatWasClimb->color = root->color;
   root->color = RED;
   return nodeThatWasClimb;
}

LPLLRB rightRotate(LPLLRB root) {
  LPLLRB nodeThatWasClimb = root->left;
  root->left = nodeThatWasClimb->right;
  nodeThatWasClimb->right = root;
  nodeThatWasClimb->color = root->color;
  root->color = RED;
  return nodeThatWasClimb;
}



int getNodeColor(LPLLRB root) {
    if (root == NULL) return BLACK;
    return root->color;
}

LPLLRB move2LeftRED(LPLLRB root) {
    switchColor(root);
    if (root != NULL && root->right != NULL && getNodeColor(root->right->left) == RED) {
        root->right = leftRotate(root->right);
        root = rightRotate(root);
        switchColor(root);
    }
    return root;
}

LPLLRB move2RightRED(LPLLRB root) {
    switchColor(root);
    if (root != NULL && root->left != NULL && getNodeColor(root->left->left) == RED) {
        root = rightRotate(root);
        switchColor(root);
    }
    return root;
}

LPLLRB reBalancing(LPLLRB root) {
    if (root != NULL && getNodeColor(root->right) == RED) {
        root = leftRotate(root);
    }
    if (root != NULL && root->left != NULL && getNodeColor(root->left) == RED && getNodeColor(root->left->left) == RED) {
        root = rightRotate(root);
    }
    if (root != NULL && getNodeColor(root->left) == RED && getNodeColor(root->right) == RED) {
        switchColor(root);
    }
    return root;
}


int searchForValue(LPLLRB root, int value) {
    if (root == NULL) return 0;
    if (root->data == value) {
        return 1;
    } else if (root->data > value) {
        return searchForValue(root->left, value);
    } else if (root->data < value) {
        return searchForValue(root->right, value);
    }
}

LPLLRB removeSmallestNode(LPLLRB root) {
    if (root->left == NULL) {
        free(root);
        return NULL;
    }
    if (getNodeColor(root->left) == BLACK && getNodeColor(root->left->left) == BLACK) {
        root = move2LeftRED(root);
    }

    root->left = removeSmallestNode(root->left);
    return reBalancing(root);
}

LPLLRB getSmallestNode(LPLLRB root) {
    if (root != NULL) {
        if (root->left != NULL) {
            return getSmallestNode(root->left);
        } else {
            return root;
        }
    } else{
        return root;
    }
}
/*
 * INSERTION RULES
 *
 * IF INSERTED IN THE EMPTY BINARY, ROOT NODE MUST BE TURNED BLACK
 * AFTER INSERTION, IF RIGHT CHILD AND LEFT CHILD ARE RED CHANGE COLORS OF THE ROOT AND CHILD
 * AFTER INSERTION, IF RIGHT CHILD ARE RED AND LEFT CHILD ARE BLACK, APPLY LEFT ROTATION IN THE ROOT
 * AFTER INSERTION, IF LEFT CHILD AND LEFT OF THE LEFT CHILD ARE RED, APPLY RIGHT ROTATION TO THE ROOT
 *
 * */

LPLLRB insertOnLLRB(LPLLRB root, int data, int * res) {
    if (root == NULL) {
        LPLLRB node = getNode(data);
        *res = 1;
        root = node;
    } else if (root->data > data) {
        root->left = insertOnLLRB(root->left, data, res);
    } else if (root->data < data) {
        root->right = insertOnLLRB(root->right, data, res);
    } else {
        *res = 0;
    }

    if (root != NULL && getNodeColor(root->left) == BLACK && getNodeColor(root->right) == RED) {
        root = leftRotate(root);
    }

    if (root != NULL && root->left != NULL && getNodeColor(root->left) == RED && getNodeColor(root->left->left) == RED) {
        root = rightRotate(root);
    }

    if (root != NULL && getNodeColor(root->left) == RED && getNodeColor(root->right) == RED ) {
        switchColor(root);
    }
    return root;
}

int insertData(LPLLRB * root, int data) {
    int res;
    (*root) = insertOnLLRB(*root, data, &res);
    if (root != NULL) {
        (*root)->color = BLACK;
    }
    return res;
}


/*
 * REMOTION RULES
 *
 * */

LPLLRB removeFromLLRB(LPLLRB *root, int data) {
    LPLLRB node = *root;

    if (node->data > data) {
        if (getNodeColor(node->left) == BLACK && getNodeColor(node->left->left) == BLACK) {
            *root = move2LeftRED(node);
        }
        node->left = removeFromLLRB(&(node->left), data);
        return node;
    } else {
        if (node->left == RED) {
            *root = move2RightRED(node);
        }

        if (node->data == data && node->right == NULL) {
            free(node);
            return NULL;
        }

        if (node->right != NULL && getNodeColor(node->right) == BLACK && getNodeColor(node->right->left) == BLACK) {
            *root = move2RightRED(node);
        }

        if (node->data == data) {
            LPLLRB smallestNode = getSmallestNode(node->right);
            node->data = smallestNode->data;
            node->right = removeSmallestNode(node->right);
        } else {
            node->right = removeFromLLRB(&(node->right), data);
        }
        return reBalancing(node);
    }
}

int removeValue(LPLLRB *root, int data) {
    if (searchForValue(*root, data)) {
        *root = removeFromLLRB(root, data);
        if (*root != NULL) {
            (*root)->color = BLACK;
        }
        return 1;
    } else {
        return 0;
    }
}

void printLLRB(LPLLRB root) {
    if (root == NULL) return;
    printLLRB(root->left);
    printf("%i ",root->data);
    printLLRB(root->right);
}
int main() {
    LPLLRB root = NULL;
    /*for (int i = 0 ; i < 10 ; i++) {
        insertData(&root, i+1);
    }*/

    insertData(&root, 50);
    insertData(&root, 5);
    insertData(&root, 530);
    insertData(&root, 25);
    insertData(&root, 3);
    insertData(&root, 1);
    insertData(&root, 9);
    insertData(&root, 700);
    insertData(&root, 232);
    insertData(&root, 1000);
//    printf("%i\n",searchForValue(root, 0));
    removeValue(&root, 1000);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 232);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 700);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 9);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 1);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 3);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 25);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 530);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 5);
    printLLRB(root);
    printf("\n");
    removeValue(&root, 50);
    printLLRB(root);
    return 0;
}
