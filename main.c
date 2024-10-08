#include <stdio.h>
#include <stdlib.h>
#include "DLL.h"
#include "fileReader.h"
#include "fifo.h"

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

    FIFO(referenceList);

    // Clean up
    freeList(referenceList);

    return EXIT_SUCCESS;
}
