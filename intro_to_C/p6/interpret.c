/**
  @file interpret.c
  @author Dr. Sturgill 
  Parse through a psuedo-code and interpret the psuedo-code to commands in C. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "value.h"
#include "syntax.h"
#include "parse.h"

/** Require 2 arguments. */
#define REQ_ARG 2

/** Print a usage message then exit unsuccessfully. */
void usage()
{
  fprintf(stderr, "usage: interpret <program-file>\n");
  exit(EXIT_FAILURE);
}

/** 
  Program starting point, reads filename from the command-line arguments.
  Parse through a psuedo-code and interpret the psuedo-code to commands in C. 
  @param argc number of command-line arguments.
  @param argv list of command-line arguments.
  @return program exit status
*/
int main(int argc, char *argv[])
{
  // Open the program's source.
  if (argc != REQ_ARG)
    usage();
  
  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  // Environment, for storing variable values.
  Environment *env = makeEnvironment();
  
  // Parse one statement at a time, then run each statement
  // using the same Environment.
  char tok[MAX_TOKEN + 1];
  while (parseToken(tok, fp)) {
    // Parse the next input statement.
    Stmt *stmt = parseStmt(tok, fp);

    // Run the statement.
    stmt->execute(stmt, env);

    // Delete the statement.
    stmt->destroy(stmt);
  }
  
  // We're done, close the input file and free the environment.
  fclose(fp);
  freeEnvironment(env);

  return EXIT_SUCCESS;
}
