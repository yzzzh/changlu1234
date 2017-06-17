#ifndef BKDR_HASH_H
#define BKDR_HASH_H
#define STRLEN 15
#define DICLEN 100000
#include <stdlib.h>
#include <string>
#include <cstring>

using namespace std;



typedef string TYPE;
typedef int BOOL;
unsigned int BKDR_HASH(TYPE);

typedef struct _NODE{
    TYPE data;
    TYPE value;
    struct _NODE* next;
}NODE;

class BKDR_Hash{
private:
    NODE* pHead;
    NODE** chainHash;
    BKDR_Hash* new_hash_table;
public:
    static unsigned int BKDR_HASH(TYPE key) {//BKDRhash函数
        unsigned int seed = 131;
        unsigned int hash = 0;

        for(char c:key)
            hash = hash * seed + c;
        return hash % DICLEN;
    }

    static NODE* create_node() {
        NODE* pNode = (NODE*)malloc(sizeof(NODE));
        memset(pNode, 0, sizeof(NODE));
        pNode->next = NULL;
        return pNode;
    }

    BKDR_Hash() {
        new_hash_table = (BKDR_Hash *) malloc(sizeof(BKDR_Hash));
        memset(new_hash_table, 0, sizeof(BKDR_Hash));

        new_hash_table->pHead = create_node();
        new_hash_table->chainHash = (NODE **) malloc(DICLEN * sizeof(NODE *));

        for (int i = 0; i < DICLEN; i++) {
            new_hash_table->chainHash[i] = (NODE *) malloc(sizeof(NODE));
            memset(new_hash_table->chainHash[i], 0, sizeof(NODE));
        }
    }

    BOOL insert(TYPE data,TYPE value) {
        if (new_hash_table == NULL)
            return 0;

        if (new_hash_table->chainHash[BKDR_HASH(data)]->data == "") {
            NODE *newNode = create_node();
            newNode->data = data;
            newNode->value = value;
            newNode->next = NULL;
            new_hash_table->chainHash[BKDR_HASH(data)]->data = newNode->data;
            new_hash_table->chainHash[BKDR_HASH(data)]->next = newNode->next;
            new_hash_table->chainHash[BKDR_HASH(data)]->value = newNode->value;
            free(newNode);
            return 1;
        } else {
            pHead = new_hash_table->chainHash[BKDR_HASH(data)];
            while (pHead->next != NULL)
                pHead = pHead->next;
            pHead->next = create_node();
            pHead->next->data = data;
            pHead->next->value = value;
            pHead->next->next = NULL;
            return 1;
        }
    }

    string operator[](const string &key) {
        if (find(key) == NULL)
            return "";
        else
            return find(key)->value;
    }

    NODE* find(TYPE data) {
        pHead = new_hash_table->chainHash[BKDR_HASH(data)];

        if(new_hash_table == NULL)
            return NULL;

        while(pHead != NULL){
            if(pHead->data == data)
                return pHead;
            else
                pHead = pHead->next;
        }
        return NULL;
    }
};



#endif