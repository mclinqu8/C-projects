/**
  @file hash.c
  @author Maggie Lin (mclin)
  This file contains the main method for the RIPEMD hash program. It reads a file 
  and store the information in a buffer. It then create a ByteBuffer from the files, 
  hash the buffer with RIPEMD algorithm,and prints out the final hash values as 
  a 160 bit number in hexadecimal.
*/

#include "byteBuffer.h"
#include "ripeMD.h"
#include <errno.h>

/** The minimum number of arguments in the command-line. */
#define ARG_MIN 2

/** The argument in the command-line which contains the filename. */
#define FILENAME_ARG 1

/** 
  Program starting point, reads filename from the command-line arguments.
  Create a ByteBuffer from the files, hash the buffer with RIPEMD algorithm,
  and prints out the final hash values as a 160 bit number in hexadecimal.
  @param argc number of command-line arguments.
  @param argv list of command-line arguments.
  @return program exit status
*/
int main(int argc, char *argv[])
{

  if (argc < ARG_MIN) {
    fprintf(stderr, "usage: hash <input-file>\n");
    exit(EXIT_FAILURE);
  }
  ByteBuffer *buffer = readFile(argv[FILENAME_ARG]);
  if (buffer == NULL) {
    perror(argv[FILENAME_ARG]);
    exit(EXIT_FAILURE);
  }

  HashState state;
  initState(&state);
  padBuffer(buffer);

  for (int i = COUNT_START; i < buffer->len / BLOCK_BYTES; i++) {
    hashBlock(&state, &buffer->data[i * BLOCK_BYTES]);
  }

  printHash(&state);
  freeBuffer(buffer);
  return EXIT_SUCCESS;
}
