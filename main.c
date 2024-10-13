#include <stdio.h>
#include <stdlib.h>
#include "DLL.h"
#include "fileReader.h"
#include "fifo.h"
#include "secondChance.h"

int main()
{

    DllList *referenceList = createList();

    const char *inputFilename = "Assignment 2 input file.csv";
    int entriesRead = readInputFile(inputFilename, referenceList);
    if (entriesRead < 0)
    {
        fprintf(stderr, "Failed to read input file.\n");
        freeList(referenceList);
        return EXIT_FAILURE;
    }

    //------------FIFO-------------
    // FIFO(referenceList);

    //----------Second Chance---------
    // n is fixed
    printf("Running SecondChanceClk Case 1:\n");
    SecondChanceClk_Case1(referenceList);

    // m is fixed
    printf("\nRunning SecondChanceClk Case 2:\n");
    SecondChanceClk_Case2(referenceList);

    // Clean up
    freeList(referenceList);

    return EXIT_SUCCESS;
}
