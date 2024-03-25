/**
  @file input.h
  @author Maggie Lin (mclin)
  This header contains a function prototype that reads in a single line from the given
  input file stream and return it as a string inside a dynamically allocated memory.
*/

#include <stdio.h>
#include <stdlib.h>

/** The initial capacity of the dynamically allocated memory for the string. */
#define INITIAL_CAPACITY 5

/** The initial length of the string. */
#define INITIAL_LENGTH 0

/** Double the capacity of the dynamically allocated memory. */
#define DOUBLE 2

/**
  Reads a single line from the given input file stream, stores and return it as a string.
  @param fp the input file stream to read from.
  @return a string of the line read from input file stream stored inside a
  dynamically allocated memory.
  */
char *readLine(FILE *fp);
