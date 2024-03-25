/** 
  @file expand.c
  @author Maggie Lin (mclin)
  This file contains a few functions that tells if character is a 
  valid word character, gives the upper bound of the given string after all target
  words are replaced by replacement strings, and make a string with all the target
  words swapped out for their replacement strings.
*/

#include "expand.h"

/**
  Check if the given character is either a letter, a decimal digit, 
  or an underscore.
  @param ch the character to check.
  @return true if the characters is a valid word character.
  */ 
bool wordChar(int ch)
{
  if (ch == '_') {
    return true;
  } else if (ch >= 'A' && ch <= 'Z') {
    return true;
  } else if (ch >= 'a' && ch <= 'z') {
    return true;
  } else if (ch >= '0' && ch <= '9') {
    return true;
  } else {
    return false;
  }
}

/**
  Returns the upper bound of the given string if all the target words
  are replaced by replacement strings.
  @param src the original string.
  @param maxRep the maximum length of the replacement strings.
  @return the upper bound of the given string if all the target words
  are replaced by replacement strings.
  */ 
int expansionBound(char *src, int maxRep)
{
  int len = INITIAL_COUNT;
  int count = INITIAL_COUNT;
  int total = INITIAL_COUNT;
  while (src[len] != '\0') {
    if (wordChar(src[len])) {
      count++;
    } else {
      if (count == INITIAL_COUNT) {
        total = total + SINGLE_CHAR;
      } else if (count >= maxRep) {
        //add one for space or anything not a word character
        total = total + count + SINGLE_CHAR;
      } else {
        //add one for space or anything not a word character
        total = total + maxRep + SINGLE_CHAR;
      }
      count = INITIAL_COUNT;
    }
    len++;
  }
  //If end with no new line or without anything not a word char
  if (count > INITIAL_COUNT) {
    if (count >= maxRep) {
      //add one for space or anything not a word character
      total = total + count; 
    } else {
      total = total + maxRep;
    }
  }
  //for null terminator
  total++;
  return total;
}

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
void expand(char *src, char *dest, char *tList[], char *rList[], int pairs)
{
  int len = INITIAL_COUNT;
  int size = INITIAL_COUNT;
  int destIdx = INITIAL_COUNT;
  bool replace = false;
  int replaceIdx = NOT_VALID;
  char word[STRING_MEM] = "";
  char replaceWord[STRING_MEM] = "";
  char currChar;
  char notWord;
  while (src[len] != '\0') {
    if (wordChar(src[len])) {
      currChar = src[len];
      word[size++] = currChar;
      word[size] = '\0';
    } else {
      notWord = src[len];
      //checks if there is a replacement for word so far
      for(int i = INITIAL_COUNT; i < pairs; i++) {
        if(strcmp(word, tList[i]) == TRUE) {
          replace = true;
          replaceIdx = i;
        }
      }
      //if there is a relacement, use replacement instead of the regular word
      if (replace) {
        strcpy(replaceWord, rList[replaceIdx]);
        for (int j = INITIAL_COUNT; j < strlen(replaceWord); j++) {
          dest[destIdx++] = replaceWord[j];
        }
        strcpy(replaceWord, "");
        replace = false;
        replaceIdx = NOT_VALID;
      } else {
        for (int k = INITIAL_COUNT; k < strlen(word); k++) {
          dest[destIdx++] = word[k];
        }
      }
      dest[destIdx++] = notWord;
      //put word back to blank to be used again
      strcpy(word, "");
      size = INITIAL_COUNT;
    }
    len++;
  }
  dest[destIdx] = '\0';
}
