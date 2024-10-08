#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"
#include "DLL.h"

void FIFO(DllList *referenceList)
{
    // for output
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

            // search through the ram to see if teh page is already in there
            Node *pageInFrames = Search(frameList, pageNum);

            if (pageInFrames == NULL)
            {
                // page not in ram so page fault
                pageFaults++;

                if (framesInUse < frameCount)
                {
                    // add page to ram since there is room for it
                    append(frameList, pageNum, dirty);
                    framesInUse++;
                }
                else
                {
                    // replace the oldest node because of FIFO
                    // oldest node will always be the first in the Doubly Linked List
                    Node *oldPageNode = frameList->left->nextNode;

                    // increment writeback if page to be replaced was dirty
                    if (oldPageNode->dirty == 1)
                    {
                        writeBacks++;
                    }

                    removeNode(frameList, oldPageNode);
                    append(frameList, pageNum, dirty);
                }
            }
            else
            {
                // page is already in Ram, just need to update dirty bit if need be
                if (dirty == 1 && pageInFrames->dirty == 0)
                {
                    pageInFrames->dirty = 1;
                }
            }

            currentRef = currentRef->nextNode;
        }
        // for output
        printf("%d\t%d\t\t%d\n", frameCount, pageFaults, writeBacks);

        // clean up the Ram (nodes on the heap)
        freeList(frameList);
    }
}
