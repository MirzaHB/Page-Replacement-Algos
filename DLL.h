#ifndef DLL_H
#define DLL_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 100
#define TOTAL_PAGES 500

typedef struct Node Node;
struct Node
{
    Node *nextNode;
    Node *prevNode;
    uint16_t pageNum;
    uint16_t dirty;
    uint32_t referenceRegister;
};
typedef struct
{
    Node *left;
    Node *right;
} DllList;

DllList *createList();
void append(DllList *list, int pageNum, int dirty);
void insertAtHead(DllList *list, int pageNum, int dirty);
void insertAtIndex(DllList *list, int index, int pageNum, int dirty);
void removeNode(DllList *list, Node *node);
void moveToHead(DllList *list, Node *node);
Node *Search(DllList *List, int pageNum);
void freeList(DllList *list);

#endif