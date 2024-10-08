#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileReader.h"
#include "DLL.h"

int readInputFile(const char *filename, DllList *list)
{
    FILE *input = fopen(filename, "r");
    if (input == NULL)
    {
        fprintf(stderr, "Error opening input file: %s\n", filename);
        return -1;
    }

    char line[256];
    int rowCount = 0;

    if (fgets(line, sizeof(line), input) == NULL)
    {
        fprintf(stderr, "Error reading header line\n");
        fclose(input);
        return -1;
    }

    while (fgets(line, sizeof(line), input))
    {

        line[strcspn(line, "\n")] = '\0';

        char *token;
        int pageNum;
        int dirty;

        token = strtok(line, ",");
        if (token == NULL)
        {
            fprintf(stderr, "Error parsing page number in line: %s\n", line);
            continue;
        }
        pageNum = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL)
        {
            fprintf(stderr, "Error parsing dirty bit in line: %s\n", line);
            continue;
        }
        dirty = atoi(token);

        append(list, pageNum, dirty);
        rowCount++;
    }

    fclose(input);
    return rowCount;
}