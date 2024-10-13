#include <stdio.h>
#include <stdlib.h>
#include "secondChance.h"
#include "DLL.h"

#define FRAME_COUNT 50

void SecondChanceClk_Case1(DllList *referenceList)
{

    int n = 8;
    printf("m\tPage Faults\tWrite backs\n");

    // runs from m=1 to m=100
    for (int m = 1; m <= 100; m++)
    {
        int pageFaults = 0;
        int writeBacks = 0;
        int framesInUse = 0;
        int referenceCounter = 0;

        // create the ram. Each node will be a frame in the ram
        DllList *frameList = createList();

        Node *currentRef = referenceList->left->nextNode;

        while (currentRef != referenceList->right)
        {
            int pageNum = currentRef->pageNum;
            int dirty = currentRef->dirty;

            // count to make sure to right shift registers correctly
            referenceCounter++;

            // check to see if page is in Ram
            Node *pageInFrames = Search(frameList, pageNum);

            if (pageInFrames == NULL)
            {
                // pagefault
                pageFaults++;
                // check to see if there is ROOM in ram for the page
                if (framesInUse < FRAME_COUNT)
                {
                    // add page to ram
                    append(frameList, pageNum, dirty);
                    framesInUse++;
                    // Set MSB of referenceRegister to 1 and mask to n bits
                    frameList->right->prevNode->referenceRegister |= (1 << (n - 1));
                }
                else
                {
                    // ram is full: find page with smallest reference register value to remove
                    Node *minNode = NULL;
                    uint32_t minValue = UINT32_MAX;
                    Node *current = frameList->left->nextNode;

                    while (current != frameList->right)
                    {
                        uint32_t refValue = current->referenceRegister;
                        if (refValue < minValue)
                        {
                            minValue = refValue;
                            minNode = current;
                        }
                        current = current->nextNode;
                    }

                    if (minNode->dirty == 1)
                    {
                        writeBacks++;
                    }

                    // removing old page from ram
                    removeNode(frameList, minNode);

                    // adding page to ram
                    append(frameList, pageNum, dirty);
                    // Set MSB of referenceRegister to 1 and mask to n bits
                    frameList->right->prevNode->referenceRegister = (1U << (n - 1));
                }
            }
            else
            {
                // page is in ram already, update dirty bit if needed
                if (dirty == 1 && pageInFrames->dirty == 0)
                {
                    pageInFrames->dirty = 1;
                }
                // set MSB of referenceRegister to 1
                pageInFrames->referenceRegister |= (1 << (n - 1));
            }

            // right shift reference registers since m references have been made
            if (referenceCounter == m)
            {
                Node *current = frameList->left->nextNode;
                while (current != frameList->right)
                {
                    current->referenceRegister >>= 1;
                    current = current->nextNode;
                }
                referenceCounter = 0;
            }

            currentRef = currentRef->nextNode;
        }

        printf("%d\t%d\t\t%d\n", m, pageFaults, writeBacks);

        // Clean up ram
        freeList(frameList);
    }
}

void SecondChanceClk_Case2(DllList *referenceList)
{
    u_int8_t m = 10;
    printf("n\tPage Faults\tWrite backs\n");

    // runs from n=1 to n=32
    for (int n = 1; n <= 32; n++)
    {
        int pageFaults = 0;
        int writeBacks = 0;
        int framesInUse = 0;
        u_int8_t referenceCounter = 0;

        // create the ram. Each node will be a frame in the ram
        DllList *frameList = createList();

        Node *currentRef = referenceList->left->nextNode;

        while (currentRef != referenceList->right)
        {
            int pageNum = currentRef->pageNum;
            int dirty = currentRef->dirty;

            // count to make sure to right shift registers correctly
            referenceCounter++;

            // check to see if page is in RAM
            Node *pageInFrames = Search(frameList, pageNum);

            if (pageInFrames == NULL)
            {
                // pagefault
                pageFaults++;
                // check to see if there is ROOM in ram for the page
                if (framesInUse < FRAME_COUNT)
                {
                    // add page to ram
                    append(frameList, pageNum, dirty);
                    framesInUse++;
                    // Set MSB of referenceRegister to 1
                    frameList->right->prevNode->referenceRegister |= (1 << (n - 1));
                }
                else
                {
                    // ram is full: find page with smallest reference register value to remove
                    Node *minNode = NULL;
                    uint32_t minValue = UINT32_MAX;
                    Node *current = frameList->left->nextNode;

                    while (current != frameList->right)
                    {
                        uint32_t refValue = current->referenceRegister;
                        if (refValue < minValue)
                        {
                            minValue = refValue;
                            minNode = current;
                        }
                        current = current->nextNode;
                    }

                    // increment writebacks if page is dirty
                    if (minNode->dirty == 1)
                    {
                        writeBacks++;
                    }

                    // removing old page from ram
                    removeNode(frameList, minNode);

                    // adding page to ram
                    append(frameList, pageNum, dirty);
                    // set MSB of referenceRegister to 1
                    frameList->right->prevNode->referenceRegister |= (1 << (n - 1));
                }
            }
            else
            {
                // page is in ram already, update dirty bit if needed
                if (dirty == 1 && pageInFrames->dirty == 0)
                {
                    pageInFrames->dirty = 1;
                }
                // set the MSB to 1
                pageInFrames->referenceRegister |= (1U << (n - 1));
            }

            // right shift reference registers since m references have been made
            if (referenceCounter == m)
            {
                Node *current = frameList->left->nextNode;
                while (current != frameList->right)
                {
                    current->referenceRegister >>= 1;
                    // perform a right shift
                    current->referenceRegister &= (1U << n) - 1;
                    current = current->nextNode;
                }
                referenceCounter = 0;
            }

            currentRef = currentRef->nextNode;
        }

        printf("%d\t%d\t\t%d\n", n, pageFaults, writeBacks);

        // Clean up ram
        freeList(frameList);
    }
}
