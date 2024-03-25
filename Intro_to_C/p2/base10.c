/** 
  @file base10.c
  @author Maggie Lin (mclin)
  This file contains a few functions that reads and writes 
  number in base ten.
*/

#include <stdio.h>
#include "base.h"

/**
  Read a number from standard input in base ten, terminating unsuccessfully
  if it's not in the right format or if there's an overflow.
  @return the number read in.
  */ 
long readNumber() {
  long val = getchar();
  ungetc(val, stdin);
  scanf("%ld", &val);
  return val;
}

/**
  Print the given value to standard output in base ten.
  @param val the value to print.
  */ 
void printNumber( long val ) {
  printf("%ld", val);
}
