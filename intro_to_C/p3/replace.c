/** 
  @file replace.c
  @author Maggie Lin (mclin)
  This file takes in an input file and output file, read the input
  file line-by-line and replace target words with their replacement 
  word and output those modified strings into the output file.
*/

#include "line.h"
#include "expand.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** Command line count. */
#define COMMAND_LINE 1

/** Number of required arguments at the end of the command line. */
#define REQUIRED_ARGS 2

/** The number by counting from the end at which the output file name is located */
#define OUTPUT 1

/** The remainder of mod two that shows a number is even. */
#define EVEN 0

/** The second index of an array. */
#define SECOND_INDEX 1

/** 
  Program starting point. Reads lines from file and replace target words with their
  replacements. Print the replaced text into the output file.
  @param argc number of command-line arguments.
  @param argv list of command-line arguments
  @return program exit status
*/
int main(int argc, char *argv[]) 
{
  int tarsize = INITIAL_COUNT;
  int repsize = INITIAL_COUNT;
  int maxRep = INITIAL_COUNT;
  char *targetptr[argc];
  char *replaceptr[argc];
  //check if there is enough arguments 
  if (argc < REQUIRED_ARGS || argc % REQUIRED_ARGS == EVEN) {
    fprintf(stderr, "usage: replace (<target> <replacement>)* <input-file> <output-file>\n");
    exit(EXIT_FAILURE);
  }
  //check if the input and output file names are valid
  FILE *src = fopen(argv[argc - REQUIRED_ARGS], "r");
  FILE *out = fopen(argv[argc - OUTPUT], "w");
  if (src == NULL) {
    fprintf(stderr, "Can't open file: %s\n", argv[argc - REQUIRED_ARGS]);
    exit(EXIT_FAILURE);
  }
  if (out == NULL) {
    fprintf(stderr, "Can't open file: %s\n", argv[argc - OUTPUT]);
    fclose(out);
    exit(EXIT_FAILURE);
  }

  //check if target and replacement words are valid and save them
  //to a pointer array
  for(int i = COMMAND_LINE; i < argc - REQUIRED_ARGS; i++) {
    for (int j = INITIAL_COUNT; j < strlen(argv[i]); j++) {
      if (!wordChar(argv[i][j])) {
        fprintf(stderr, "Invalid word: %s\n", argv[i]);
        exit(EXIT_FAILURE);
      }
    }
    if (i % REQUIRED_ARGS != EVEN) {
      targetptr[tarsize++] = argv[i];
    } else {
      replaceptr[repsize++] = argv[i];
    }
  }

  //Check for replacement chains
  for (int a = INITIAL_COUNT; a < repsize; a++) {
    for (int b = INITIAL_COUNT; b < tarsize; b++) {
      if (strcmp(replaceptr[a], targetptr[b]) == TRUE) {
        replaceptr[a] = replaceptr[b];
        if (strcmp(targetptr[a], replaceptr[a]) == TRUE) {
          fprintf(stderr, "Replacement cycle\n");
          exit(EXIT_FAILURE);
        }
        a = INITIAL_COUNT;
      }
    }
  }

  //Check for duplicate target word
  for (int k = INITIAL_COUNT; k < tarsize - SECOND_INDEX; k++) {
    for (int l = k + SECOND_INDEX; l < tarsize; l++) {
      if (strcmp(targetptr[k], targetptr[l]) == TRUE) {
        fprintf(stderr, "Duplicate target: %s\n", targetptr[k]);
        exit(EXIT_FAILURE);
      }
    }
  }

  //Check for longest replacement word length
  for (int m = INITIAL_COUNT; m < repsize; m++) {
    if (strlen(replaceptr[m]) > maxRep) {
      maxRep = strlen(replaceptr[m]);
    }
  }

  //Proceeds to replace all target words with their replacements
  while(measureLine(src) != INITIAL_COUNT) {
    //add a single char for null terminator
    char currString[measureLine(src) + SINGLE_CHAR];
    readLine(src, currString);
    char destString[expansionBound(currString, maxRep)];
    expand(currString, destString, targetptr, replaceptr, tarsize);
    fprintf(out, destString);
  }
  //Close file to prevent memory leak
  fclose(src);
  fclose(out);
  return EXIT_SUCCESS;
}


