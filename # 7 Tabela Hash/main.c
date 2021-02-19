#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct CustomerData {
    int id;
    char *name;
    struct CustomerData *next;
} *LPCustomerData;

typedef struct HashTable {
    int tableSize;
    LPCustomerData *customerData;
} *LPHashTable;

int stringLength(const char *string) {
    int length = 0;
    while (string[length] != '\n') length++;
    return length;
}

LPCustomerData generateCustomerDataNode(int id, char *name) {
    LPCustomerData newCustomerData = NULL;
    newCustomerData = (LPCustomerData) malloc(sizeof(struct CustomerData));
    newCustomerData->name = (char *) malloc(sizeof(char) * stringLength(name));
    newCustomerData->id = id;
    strcpy(newCustomerData->name, name);
    newCustomerData->next = NULL;
    return newCustomerData;
}

int generateHashTable(LPHashTable *hashTable, int tableSize) {
    LPHashTable newHashTable = (LPHashTable) malloc(sizeof(struct HashTable));
    if (newHashTable == NULL) return 0; /*erro*/
    newHashTable->tableSize = tableSize;
    newHashTable->customerData = (LPCustomerData *) malloc(newHashTable->tableSize * sizeof(LPCustomerData));
    if (newHashTable->customerData == NULL) return 0; /*erro*/
    for (int i = 0; i < newHashTable->tableSize; i++) {
        newHashTable->customerData[i] = NULL;
    }
    *hashTable = newHashTable;
    return 1; /*success*/
}

int posByDivision(int value, int tableSize) {
    return (value & 0x7FFFFFFF) % tableSize;
}

int insertInHashtable(LPHashTable hashTable, int id, char *name) {
    int pos, nextPos = 0;
    LPCustomerData newCustomerData = NULL, customerWard = NULL;
    if (hashTable == NULL) return 0; /* Error */
    newCustomerData = generateCustomerDataNode(id, name);

    if (newCustomerData == NULL) return 0; /* Error */
    pos = posByDivision(id, hashTable->tableSize);

    if (hashTable->customerData[pos] != NULL) {
        customerWard = hashTable->customerData[pos];
        while(customerWard->next != NULL && (nextPos++) < hashTable->tableSize/2) customerWard = customerWard->next;
        customerWard->next = newCustomerData;
    } else {
        hashTable->customerData[pos] = newCustomerData;
    }

    return 1;
}

int removeFromHashTable(LPHashTable hashTable, int idToRemove) {
    int pos;
    LPCustomerData customerData = NULL, customerToRemove = NULL;
    if (hashTable == NULL) return 0; /* Error */
    pos = posByDivision(idToRemove, hashTable->tableSize);
    if (hashTable->customerData[pos] == NULL) return 0; /* Error */
    customerData = hashTable->customerData[pos];
    if (customerData->id != idToRemove) {
        while (customerData->next != NULL && customerData->next->id != idToRemove) customerData = customerData->next;
        if (customerData->next == NULL) return 0; /* Error */
        customerToRemove = customerData->next;
        customerData->next = customerToRemove->next;
        free(customerToRemove);
        return 1;
    } else {
        customerToRemove = customerData;
        hashTable->customerData[pos] = customerData->next;
        free(customerToRemove);
        return 1;
    }
}

int findInHashTable(LPHashTable hashTable, int idToFind, LPCustomerData *customerData) {
    int pos, tableSize;
    LPCustomerData customerDataAux = NULL;
    if (hashTable == NULL) return 0; /* Error */
    tableSize = hashTable->tableSize;
    pos = posByDivision(idToFind, tableSize);
    customerDataAux = hashTable->customerData[pos];
    if (customerDataAux == NULL) return 0 ; /* Error */
    if (customerDataAux->id != idToFind) {
        while (customerDataAux != NULL && customerDataAux->id != idToFind) customerDataAux = customerDataAux->next;
        if (customerDataAux == NULL) return 0; /* Error */
        *customerData = customerDataAux;
        return 1;
    } else {
        *customerData = customerDataAux;
        return 1;
    }
}
void insertData(LPHashTable hashTable, int id, char *name) {
    if (insertInHashtable(hashTable, id, name)) {
        printf("[INSERTION] ID => %i - Ok\n", id);
    } else {
        printf("[INSERTION] ID => %i - Fail\n", id);
    }
}

void removeData(LPHashTable hashTable, int id) {
    if (removeFromHashTable(hashTable, id)) {
        printf("[REMOTION] ID => %i - Ok\n", id);
    } else {
        printf("[REMOTION] ID => %i - Fail\n", id);
    }
}

int main() {
    LPHashTable hashTable = NULL;
    LPCustomerData customerData = NULL;
    int tableSize = 10;
    generateHashTable(&hashTable, tableSize);

    insertData(hashTable, 1, "Reberth");
    insertData(hashTable, 2, "Reberth");
    insertData(hashTable, 3, "Reberth");
    insertData(hashTable, 4, "Reberth");
    insertData(hashTable, 1, "Reberth");
    insertData(hashTable, 2, "Reberth");
    insertData(hashTable, 3, "Reberth");
    insertData(hashTable, 4, "Reberth");
    removeData(hashTable, 2);
    removeData(hashTable, 2);

    if (findInHashTable(hashTable, 2, &customerData)) {
        printf("[FIND] Ok!\n");
    } else {
        printf("[FIND] Fail!\n");
    }

    for (int i = 0; i < hashTable->tableSize; i++) {
        if (hashTable->customerData[i] != NULL) {
            customerData = hashTable->customerData[i];
            printf("Pos = %i - ID = %i - Children? %i \n", i, customerData->id, customerData->next != NULL);
        }
    }
}