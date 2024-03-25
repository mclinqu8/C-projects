/** 
  @file base.h
  @author Maggie Lin (mclin)
  This header contains a few function prototypes that reads and writes 
  number in current base.
*/

#include <stdio.h>
#include "bounds.h"

/** Exit status indicating that the program was given invalid input. */
#define FAIL_INPUT 102

/**
  Read a number from standard input in the current base, terminating unsuccessfully
  if it's not in the right format or if there's an overflow.
  @return the number read in.
  */ 
long readNumber();

/**
  Print the given value to standard output in the current base.
  @param val the value to print.
  */ 
void printNumber( long val );
