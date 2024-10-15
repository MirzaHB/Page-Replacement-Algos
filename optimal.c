#include <stdio.h>
#include <stdlib.h>
#include "optimal.h"
#include "DLL.h"

// find next reference of page in reference list
int findNextUse(Node *currentRef, int pageNum, int currentPosition, int totalReferences)
{
    int position = currentPosition + 1;
    while (currentRef != NULL && position < totalReferences)
    {
        if (currentRef->pageNum == pageNum)
        {
            return position;
        }
        currentRef = currentRef->nextNode;
        position++;
    }
    // If the page is not found in the future references, return a large number
    return 99999;
}

void Optimal(DllList *referenceList)
{
    printf("Frames\tPage Faults\tWrite backs\n");

    int totalReferences = 0;
    Node *tempRef = referenceList->left->nextNode;
    while (tempRef != referenceList->right)
    {
        totalReferences++;
        tempRef = tempRef->nextNode;
    }

    for (int frameCount = 1; frameCount <= MAX_FRAMES; frameCount++)
    {
        int pageFaults = 0;
        int writeBacks = 0;
        int framesInUse = 0;
        int currentPosition = 0;

        // this represents ram
        DllList *frameList = createList();

        Node *currentRef = referenceList->left->nextNode;

        while (currentRef != referenceList->right)
        {
            int pageNum = currentRef->pageNum;
            int dirty = currentRef->dirty;

            // search for the page in RAM
            Node *pageInFrames = Search(frameList, pageNum);

            if (pageInFrames != NULL)
            {
                // Page is already in RAM update dirty bit if necessary
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
                    // space in RAM, add the page
                    append(frameList, pageNum, dirty);
                    framesInUse++;
                }
                else
                {
                    Node *currentFrameNode = frameList->left->nextNode;
                    Node *pageToReplace = NULL;
                    int farthestNextUse = -1;

                    while (currentFrameNode != frameList->right)
                    {
                        int nextUsePosition = findNextUse(currentRef->nextNode, currentFrameNode->pageNum, currentPosition, totalReferences);
                        if (nextUsePosition > farthestNextUse)
                        {
                            farthestNextUse = nextUsePosition;
                            pageToReplace = currentFrameNode;
                        }
                        currentFrameNode = currentFrameNode->nextNode;
                    }

                    // replace the pageToReplace with the new page
                    if (pageToReplace->dirty == 1)
                    {
                        writeBacks++;
                    }

                    // remove the page to replace from RAM
                    removeNode(frameList, pageToReplace);

                    // add the new page to RAM
                    append(frameList, pageNum, dirty);
                }
            }

            currentRef = currentRef->nextNode;
            currentPosition++;
        }

        // output results for current frame count
        printf("%d\t%d\t\t%d\n", frameCount, pageFaults, writeBacks);

        // garbage collection cause heap usage
        freeList(frameList);
    }
}