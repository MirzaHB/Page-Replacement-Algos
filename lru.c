#include <stdio.h>
#include <stdlib.h>
#include "lru.h"

// LRU using stack inplementation
void LRU(DllList *referenceList)
{
    printf("Frames\tPage Faults\tWrite backs\n");

    for (int frameCount = 1; frameCount <= MAX_FRAMES; frameCount++)
    {
        int pageFaults = 0;
        int writeBacks = 0;
        int framesInUse = 0;

        // this frame list will represent the ram, where each node is a frame
        DllList *frameList = createList();

        Node *currentRef = referenceList->left->nextNode;

        while (currentRef != referenceList->right)
        {
            int pageNum = currentRef->pageNum;
            int dirty = currentRef->dirty;

            // serach to see if page is in ram
            Node *pageInFrames = Search(frameList, pageNum);

            if (pageInFrames != NULL)
            {
                // page in ram so we move it to head since it was most recently used
                moveToHead(frameList, pageInFrames);

                // see if dirty bit needs to be updated
                if (dirty == 1 && pageInFrames->dirty == 0)
                {
                    pageInFrames->dirty = 1;
                }
            }
            else
            {
                // pagefault
                pageFaults++;

                if (framesInUse < frameCount)
                {
                    // add page in ram if theres space
                    insertAtHead(frameList, pageNum, dirty);
                    framesInUse++;
                }
                else
                {
                    // ram is full: Remove the node at the end of the DLL
                    Node *lruNode = frameList->right->prevNode;

                    // check to see if we need to writeback
                    if (lruNode->dirty == 1)
                    {
                        writeBacks++;
                    }

                    // thsi removes the node at the end of ram
                    removeNode(frameList, lruNode);

                    // add referenced page to ram (at the head since it was most recently used)
                    insertAtHead(frameList, pageNum, dirty);
                }
            }

            currentRef = currentRef->nextNode;
        }

        printf("%d\t%d\t\t%d\n", frameCount, pageFaults, writeBacks);

        // garbage collection for ram
        freeList(frameList);
    }
}
