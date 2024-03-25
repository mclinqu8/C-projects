/**
  @file input.c
  @author Maggie Lin (mclin)
  This file contains a function prototype that reads in a single line from the given
  input file stream and return it as a string inside a dynamically allocated memory.
*/

#include "input.h"

char *readLine(FILE *fp)
{
    char ch;
    int len = INITIAL_LENGTH;
    int capacity = INITIAL_CAPACITY;
    char *line = (char *)malloc(capacity * sizeof(char));
    while (fscanf(fp, "%c", &ch) != EOF) {
        if (len >= capacity) {
            capacity *= DOUBLE;
            line = (char *)realloc(line, capacity * sizeof(char));
        }
        if (ch == '\n') {
            break;
        }
        line[len++] = ch;
    }
    if (len == INITIAL_LENGTH) {
        free(line);
        return NULL;
    }
    line[len] = '\0';
    return line;
}
