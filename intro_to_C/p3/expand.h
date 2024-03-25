/** 
  @file expand.h
  @author Maggie Lin (mclin)
  This header contains a few function prototypes that tells if character is a 
  valid word character, gives the upper bound of the given string after all target
  words are replaced by replacement strings, and make a string with all the target
  words swapped out for their replacement strings.
*/

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** The start of counter variables. */
#define INITIAL_COUNT 0

/** The start of strings. */
#define STRING_START 0

/** The truth value of true. */
#define TRUE 0

/** The size of a single character. */
#define SINGLE_CHAR 1

/** Not a valid index or count. */
#define NOT_VALID -1

/** Space allocated for a single string. */
#define STRING_MEM 200

/**
  Check if the given character is either a letter, a decimal digit, 
  or an underscore.
  @param ch the character to check.
  @return true if the characters is a valid word character.
  */ 
bool wordChar(int ch);

/**
  Returns the upper bound of the given string if all the target words
  are replaced by replacement strings.
  @param src the original string.
  @param maxRep the maximum length of the replacement strings.
  @return the upper bound of the given string if all the target words
  are replaced by replacement strings.
  */ 
int expansionBound(char *src, int maxRep);

/**
  Creates a copy of the source string, but with every occurrence of a 
  target string replaced by its replacement. 
  @param src the original string.
  @param dest the string to store the original string with all the 
  target string replaced by its replacement.
  @param tList the list with all the target strings.
  @param rList the list with all the replacement strings.
  @param pairs the number of target and replacement pairs.
  */ 
void expand(char *src, char *dest, char *tList[], char *rList[], int pairs);
