/** 
  @file line.c
  @author Maggie Lin (mclin)
  This file contains a few function that tell the size
  that a string array needs to be to store a line and read lines of text 
  from the input.
*/

#include "line.h"

/**
  Counts the number of characters in the next line within the given file.
  @param fp the file to read from.
  @return the of characters in the next line within the given file.
  */ 
int measureLine(FILE *fp)
{
  long originalPoint;
  char current;
  int length = INITIAL_COUNT;
  originalPoint = ftell(fp);
  while ((current = fgetc(fp)) != EOF){
    length++;
    if (current == '\n'){
      break;
    }
  }
  fseek(fp, originalPoint, SEEK_SET);
  return length;
}

/**
  Reads the next line from the given file and stores it into a string.
  @param fp the file to read from.
  @param str the string to store the line read from file.
  */ 
void readLine(FILE *fp, char str[])
{
  char current;
  int len = INITIAL_COUNT;
  while((current = fgetc(fp)) != EOF) {
    str[len++] = current;
    if (current == '\n'){
      break;
    }
  }
  str[len] = '\0';
}
