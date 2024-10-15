#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DLL.h"
#include "fileReader.h"
#include "fifo.h"
#include "optimal.h"
#include "lru.h"
#include "secondChance.h"

int main(int argc, char *argv[])
{
    // make sure correct number of args were passed in
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <algorithm> <input_file>\n", argv[0]);
        fprintf(stderr, "Available algorithms: fifo, optimal, lru, secondChance\n");
        return EXIT_FAILURE;
    }

    // read command line
    const char *algorithm = argv[1];
    const char *inputFilename = argv[2];

    DllList *referenceList = createList();

    int entriesRead = readInputFile(inputFilename, referenceList);
    if (entriesRead < 0)
    {
        fprintf(stderr, "Failed to read input file: %s\n", inputFilename);
        freeList(referenceList);
        return EXIT_FAILURE;
    }

    // figuring out what algo to run
    if (strcmp(algorithm, "fifo") == 0)
    {
        FIFO(referenceList);
    }
    else if (strcmp(algorithm, "lru") == 0)
    {
        LRU(referenceList);
    }
    else if (strcmp(algorithm, "secondChance") == 0)
    {
        // make sure to run both second chance functions
        printf("Running SecondChanceClk Case 1:\n");
        SecondChanceClk_Case1(referenceList);

        printf("\nRunning SecondChanceClk Case 2:\n");
        SecondChanceClk_Case2(referenceList);
    }
    else if (strcmp(algorithm, "optimal") == 0)
    {
        Optimal(referenceList);
    }
    else
    {
        fprintf(stderr, "Unknown algorithm: %s\n", algorithm);
        fprintf(stderr, "Available algorithms: fifo, optimal, lru, secondChance\n");
        freeList(referenceList);
        return EXIT_FAILURE;
    }

    // garbage collection cause heap was used
    freeList(referenceList);

    return EXIT_SUCCESS;
}
