/** 
  @file base32.c
  @author Maggie Lin (mclin)
  This file contains a few functions that reads and writes 
  number in base thirty two.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "base.h"

/** Convert 0-9 in base 10 to base 32 */
#define DIGIT_CONVERSION 48

/** Convert 10-31 in base 10 to base 32 */
#define LETTER_CONVERSION 55

/** Number that represent base 32 */
#define BASE_32 32

/** Upper bound of base 32 */
#define BASE_32_UPPER_BOUND 31

/** Upper bound of base 10 */
#define BASE_10_UPPER_BOUND 9

/**
 Read a number from standard input in base thirty two, terminating unsuccessfully
  if it's not in the right format or if there's an overflow.
  @return the number read in.
  */ 
long readNumber() {
  //Value we've parsed so far.
  long val = ZERO_BOUND;
  long digit;
  bool negative = false;
  // Get the next input character.
  char ch = getchar();
  while (ch != '\n' && ch != ' ') {
    if ((ch < '0' || ch > 'V') && ch != '-') {
      exit(FAIL_INPUT);
    }
    if (ch == '-') {
      negative = true;
      ch = getchar();
    }
    if (ch >= '0' && ch <= '9') {
      digit = ch - DIGIT_CONVERSION;
    } else {
      digit = ch - LETTER_CONVERSION;
    }
    checkMul(val, BASE_32);
    val = val * BASE_32;
    checkAdd(val, digit);
    val = val + digit;
    ch = getchar();
  }
  ungetc(ch, stdin);
  if (negative) {
    val = -val;
  }
  return val;
}

/**
  Print the given value to standard output in base thirty two.
  @param val the value to print.
  */ 
void printNumber( long val ) {   
  char ch;

  if (val < ZERO_BOUND) {
    printf("-");
    if (val == LONG_MIN) {
      //LONG_MIN is special case where if we try printNumber(- val), it would try 
      // to do absolute value of LONG_MIN -- causing an overflow because number would
      // equal LONG_MAX + 1 which overflows to -1 (infinite loop of printing "-").
      printf("8000000000000");
    } else {
      printNumber(- val);
    }
    return;
  }

  if (val >= ZERO_BOUND && val < BASE_32_UPPER_BOUND) {
    val = val % BASE_32;
    if (val >= ZERO_BOUND && val <= BASE_10_UPPER_BOUND) {
      ch = val + DIGIT_CONVERSION;
      printf("%c", ch);
    } else {
      ch = val + LETTER_CONVERSION;
      printf("%c", ch);
    }
  } else {
    printNumber(val / BASE_32);
    val = val % BASE_32;
    if (val >= ZERO_BOUND && val <= BASE_10_UPPER_BOUND) {
      ch = val + DIGIT_CONVERSION;
      printf("%c", ch);
    } else {
      ch = val + LETTER_CONVERSION;
      printf("%c", ch);
    }
  }
}
