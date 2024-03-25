/** 
  @file bounds.h
  @author Maggie Lin (mclin)
  This header contains a few function prototypes to check if the following
  arithmetic operations will result in overflow and exit program with the 
  correct exit status if it does.
*/

/** Exit status indicating that there was overflow. */
#define NUM_OVERFLOW 100

/** Exit status indicating that there was divide-by-zero case. */
#define DIVIDE_BY_ZERO 101

/** Zero bound to help check if number is either positive or negative. */
#define ZERO_BOUND 0

/** Negative one divide case to check for overflow with division. */
#define DIVIDE_BY_NEG_ONE -1


/**
  Checks if a + b will overflow, and exit the program with the correct exit status 
  if it does.
  @param a the first value in the summation expression.
  @param b the second value in the summation expression.
  */ 
void checkAdd( long a, long b );

/**
  Checks if a - b will overflow, and exit the program with the correct exit status 
  if it does.
  @param a the first value in the subtraction expression.
  @param b the second value in the subtraction expression.
  */ 
void checkSub( long a, long b );

/**
  Checks if a * b will overflow, and exit the program with the correct exit status 
  if it does.
  @param a the first value in the multiplication expression.
  @param b the second value in the multiplication expression.
  */ 
void checkMul( long a, long b );

/**
  Checks if a / b will overflow or is a divide by zero, and exit the program with
  the correct exit status if it does.
  @param a the first value in the division expression.
  @param b the second value in the division expression.
  */ 
void checkDiv( long a, long b );
