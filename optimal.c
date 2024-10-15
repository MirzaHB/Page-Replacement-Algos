// optimal.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "optimal.h"
#include <string.h>
#include "DLL.h"

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

    /*
        referenceArray will hold the sequence of the pages numbers as they are refrenced
        and nextUse will hold when each page in referenceArray will be used again in teh future
        dirtybit array just to make sure dirtybit updates for writebacks are happening correctly
    */
    int *referenceArray = (int *)malloc(sizeof(int) * totalReferences);
    int *dirtyArray = (int *)malloc(sizeof(int) * totalReferences);
    int *nextUse = (int *)malloc(sizeof(int) * totalReferences);

    // create lastSeen array
    int lastSeen[TOTAL_PAGES];
    memset(lastSeen, -1, sizeof(lastSeen));

    // populate referenceArray and dirtyArray
    tempRef = referenceList->left->nextNode;
    int index = 0;
    while (tempRef != referenceList->right)
    {
        int pageNum = tempRef->pageNum;
        if (pageNum >= TOTAL_PAGES)
        {
            fprintf(stderr, "Page number %d exceeds MAX_PAGE_NUM %d. Increase MAX_PAGE_NUM.\n", pageNum, TOTAL_PAGES);
            exit(EXIT_FAILURE);
        }
        referenceArray[index] = pageNum;
        dirtyArray[index] = tempRef->dirty;
        index++;
        tempRef = tempRef->nextNode;
    }

    // build nextUse array with help of referenceArray
    for (int i = totalReferences - 1; i >= 0; i--)
    {
        int pageNum = referenceArray[i];
        nextUse[i] = lastSeen[pageNum];
        lastSeen[pageNum] = i;
    }

    // Arrays that track pages in ram and whenn they will be used next
    int inRAM[TOTAL_PAGES];
    int currentNextUse[TOTAL_PAGES];

    for (int frameCount = 1; frameCount <= MAX_FRAMES; frameCount++)
    {
        int pageFaults = 0;
        int writeBacks = 0;
        int framesInUse = 0;

        // should prob just use memsetfor this
        for (int i = 0; i < TOTAL_PAGES; i++)
        {
            inRAM[i] = 0;
            currentNextUse[i] = -1;
        }

        // creating ram
        DllList *frameList = createList();

        for (int currentPosition = 0; currentPosition < totalReferences; currentPosition++)
        {
            int pageNum = referenceArray[currentPosition];
            int dirty = dirtyArray[currentPosition];

            // figure out when page is used again next
            int nextUsePosition = nextUse[currentPosition];
            if (nextUsePosition == -1)
            {
                nextUsePosition = INT_MAX; // high num cause page is not used again
            }

            if (inRAM[pageNum])
            {
                // Page is in RAM
                Node *pageInFrames = Search(frameList, pageNum);
                if (pageInFrames == NULL)
                {
                    fprintf(stderr, "Error: Page %d is marked in RAM but not found in frame list.\n", pageNum);
                    exit(EXIT_FAILURE);
                }

                // Update dirty bit if necessary
                if (dirty == 1 && pageInFrames->dirty == 0)
                {
                    pageInFrames->dirty = 1;
                }

                // uodate the nextUseposistion
                currentNextUse[pageNum] = nextUsePosition;
            }
            else
            {
                // pagefault
                pageFaults++;

                if (framesInUse < frameCount)
                {
                    // sapce in ram so add referenced page
                    append(frameList, pageNum, dirty);
                    inRAM[pageNum] = 1;
                    currentNextUse[pageNum] = nextUsePosition;
                    framesInUse++;
                }
                else
                {
                    // ram is full; need to replace a page
                    int farthestNextUse = -1;
                    int pageToReplaceNum = -1;
                    Node *pageToReplaceNode = NULL;

                    // find page to replace
                    Node *currentNode = frameList->left->nextNode;
                    while (currentNode != frameList->right)
                    {
                        int nodePageNum = currentNode->pageNum;
                        int nodeNextUse = currentNextUse[nodePageNum];

                        if (nodeNextUse > farthestNextUse)
                        {
                            farthestNextUse = nodeNextUse;
                            pageToReplaceNum = nodePageNum;
                            pageToReplaceNode = currentNode;
                        }

                        currentNode = currentNode->nextNode;
                    }

                    // increment writebacks if needed for page to be replaced
                    if (pageToReplaceNode->dirty == 1)
                    {
                        writeBacks++;
                    }
                    // remove page from ram
                    removeNode(frameList, pageToReplaceNode);
                    inRAM[pageToReplaceNum] = 0;
                    currentNextUse[pageToReplaceNum] = -1;

                    // add the new page
                    append(frameList, pageNum, dirty);
                    inRAM[pageNum] = 1;
                    currentNextUse[pageNum] = nextUsePosition;
                }
            }
        }

        printf("%d\t%d\t\t%d\n", frameCount, pageFaults, writeBacks);

        // garbage collection for ram
        freeList(frameList);
    }

    // garbage collection for all the helper arrays on heap
    free(referenceArray);
    free(dirtyArray);
    free(nextUse);
}
