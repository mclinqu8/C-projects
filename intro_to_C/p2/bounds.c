/** 
  @file bounds.h
  @author Maggie Lin (mclin)
  This program contains a few function to check if the following
  arithmetic operations will result in overflow and exit program with the 
  correct exit status if it does.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bounds.h"

/**
  Checks if a + b will overflow, and exit the program with the correct exit status 
  if it does.
  @param a the first value in the summation expression.
  @param b the second value in the summation expression.
  */ 
void checkAdd( long a, long b ) {
  long result = a + b;
  //Check if sum of two positive numbers ends with a negative number
  if (a > ZERO_BOUND && b > ZERO_BOUND && result < ZERO_BOUND) {
    exit(NUM_OVERFLOW);
  } 
  //Check if sum of two negative numbers ends with a positive number
  if (a < ZERO_BOUND && b < ZERO_BOUND && result > ZERO_BOUND) {
    exit(NUM_OVERFLOW);
  }
  //Checking other sum cases
  if (a > ZERO_BOUND && b > LONG_MAX - a ) {
    exit(NUM_OVERFLOW);
  } else if (a < ZERO_BOUND && b < LONG_MIN - a) {
    exit(NUM_OVERFLOW);
  }
}

/**
  Checks if a - b will overflow, and exit the program with the correct exit status 
  if it does.
  @param a the first value in the subtraction expression.
  @param b the second value in the subtraction expression.
  */ 
void checkSub( long a, long b ) {
  signed long final = a - b;
  if (a > ZERO_BOUND && b < ZERO_BOUND && final < ZERO_BOUND) {
    exit(NUM_OVERFLOW);
  }
  if (a < ZERO_BOUND && b > ZERO_BOUND && final > ZERO_BOUND) {
    exit(NUM_OVERFLOW);
  }
  //Checking other subtraction cases
  if (a > ZERO_BOUND && b < LONG_MAX + a ) {
    exit(NUM_OVERFLOW);
  } else if (a < ZERO_BOUND && b > LONG_MIN + a) {
    exit(NUM_OVERFLOW);
  }
}

/**
  Checks if a * b will overflow, and exit the program with the correct exit status 
  if it does.
  @param a the first value in the multiplication expression.
  @param b the second value in the multiplication expression.
  */ 
void checkMul( long a, long b ) {
  long result;
  long multiple;

  if ((a > ZERO_BOUND && b > ZERO_BOUND)) {
    result = LONG_MAX / b;
    if (a > result) {
      exit(NUM_OVERFLOW);
    }
  }
  if ((a < ZERO_BOUND && b < ZERO_BOUND)) {
    result = LONG_MAX / b;
    if (a < result) {
      exit(NUM_OVERFLOW);
    }
  }
  result = a * b;
  multiple = result / b;
  if (a != multiple) {
      exit(NUM_OVERFLOW);
  }
}

/**
  Checks if a / b will overflow or is a divide by zero, and exit the program with
  the correct exit status if it does.
  @param a the first value in the division expression.
  @param b the second value in the division expression.
  */ 
void checkDiv( long a, long b ) {
  if (b == ZERO_BOUND) {
    exit(DIVIDE_BY_ZERO);
  }
  //Check overflow past LONG_MAX
  if ((a == LONG_MIN) && (b == DIVIDE_BY_NEG_ONE)) {
    exit(NUM_OVERFLOW);
  }
}

