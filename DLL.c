#include "DLL.h"

DllList *createList()
{
    DllList *list = (DllList *)malloc(sizeof(Node));
    list->left = (Node *)malloc(sizeof(Node));
    list->right = (Node *)malloc(sizeof(Node));

    list->left->nextNode = list->right;
    list->left->prevNode = NULL;

    list->right->nextNode = NULL;
    list->right->prevNode = list->left;
    return list;
}

void append(DllList *list, int pageNum, int dirty)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->pageNum = pageNum;
    newNode->dirty = dirty;
    newNode->referenceRegister = 0;

    newNode->nextNode = list->right;
    newNode->prevNode = list->right->prevNode;

    list->right->prevNode->nextNode = newNode;
    list->right->prevNode = newNode;
}

void insertAtHead(DllList *list, int pageNum, int dirty)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->pageNum = pageNum;
    newNode->dirty = dirty;

    newNode->nextNode = list->left->nextNode;
    newNode->prevNode = list->left;

    list->left->nextNode->prevNode = newNode;
    list->left->nextNode = newNode;
}

// this is never used i think* so it can prob be removed
void insertAtIndex(DllList *list, int index, int pageNum, int dirty)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->pageNum = pageNum;
    newNode->dirty = dirty;

    Node *current = list->left->nextNode;
    int currentIndex = 0;

    while (current != list->right && currentIndex < index)
    {
        current = current->nextNode;
        currentIndex++;
    }

    newNode->nextNode = current;
    newNode->prevNode = current->prevNode;

    current->prevNode->nextNode = newNode;
    current->prevNode = newNode;
}

void removeNode(DllList *list, Node *node)
{
    if (node == NULL || node == list->left || node == list->right)
    {
        return;
    }

    node->prevNode->nextNode = node->nextNode;
    node->nextNode->prevNode = node->prevNode;

    free(node);
}

void moveToHead(DllList *list, Node *node)
{
    if (node == NULL || node == list->left->nextNode)
    {
        // node is at head or null
        return;
    }

    // disconnecting node from prev location
    node->prevNode->nextNode = node->nextNode;
    node->nextNode->prevNode = node->prevNode;

    // moving node to head
    node->nextNode = list->left->nextNode;
    node->prevNode = list->left;

    list->left->nextNode->prevNode = node;
    list->left->nextNode = node;
}

Node *Search(DllList *List, int pageNum)
{
    Node *current = List->left->nextNode;
    while (current != List->right)
    {
        if (current->pageNum == pageNum)
        {
            return current;
        }
        current = current->nextNode;
    }
    return NULL;
}

void freeList(DllList *list)
{
    Node *current = list->left;
    while (current != NULL)
    {
        Node *nextNode = current->nextNode;
        free(current);
        current = nextNode;
    }
    free(list);
}