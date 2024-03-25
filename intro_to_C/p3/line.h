/** 
  @file line.h
  @author Maggie Lin (mclin)
  This header contains a few function prototypes that tell the size
  that a string array needs to be to store a line and read lines of text 
  from the input.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** The start of counter variables. */
#define INITIAL_COUNT 0

/**
  Counts the number of characters in the next line within the given file.
  @param fp the file to read from.
  @return the of characters in the next line within the given file.
  */ 
int measureLine(FILE *fp);

/**
  Reads the next line from the given file and stores it into a string.
  @param fp the file to read from.
  @param str the string to store the line read from file.
  */ 
void readLine(FILE *fp, char str[]);
