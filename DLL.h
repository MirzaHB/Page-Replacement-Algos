#ifndef DLLIST_H
#define DLLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    Node *nextNode;
    Node *prevNode;
    int pageNum;
    int dirty;
} Node;

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

#endif