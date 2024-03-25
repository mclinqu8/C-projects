/**
  @file byteBuffer.h
  @author Maggie Lin (mclin)
  This file contains the struct for ByteBuffer. The file also contains a few function 
  prototypes that initialize the ByteBuffer fields, free the memory used to store 
  ByteBuffer and data array contained within ByteBuffer, read a file stream and store 
  the information within the file in ByteBuffer, and add bytes to the ByteBuffer.
*/

#ifndef _BYTE_BUFFER_H_
#define _BYTE_BUFFER_H_
#include <stdio.h>
#include <stdlib.h>

/** Number of bits in a byte */
#define BBITS 8

/** Initial capacity of dynamically allocated memory */
#define INITIAL_CAPACITY 5

/** Start of count */
#define COUNT_START 0

/** Double capacity every time dynamically allocated memory needs
    to be increased */
#define DOUBLE 2

/** Type used as a byte. */
typedef unsigned char byte;

/** Representation for a file copied to memory, with some padding
    at the end. */
typedef struct
{
  /** Array of bytes from the file (no null termination, so it's not a
      string). */
  byte *data;

  /** Number of currently used bytes in the data array. */
  unsigned int len;

  /** Capacity of the data array (it's typically over-allocated). */
  unsigned int cap;
} ByteBuffer;

/**
  Dynamically allocates a ByteBuffer struct and initializes its fields
  to represent an empty resizable array of bytes.
  @return ByteBuffer which can act as a buffer to hold information to be hashed. 
*/
ByteBuffer *createBuffer();

/**
  Adds a single byte to the end of the given buffer, enlarging the
  data array if necessary.
  @param buffer ByteBuffer struct to add given byte to.
  @param b byte to add to the given ByteBuffer.
*/
void addByte(ByteBuffer *buffer, byte b);

/**
  Frees all the memory for the given buffer.
  @param buffer ByteBuffer struct to free all memories to.
*/
void freeBuffer(ByteBuffer *buffer);

/**
  Creates a new ByteBuffer and initializes the contents of the data
  array with the contents of the given file. If the file can't be opened,
  it will just return NULL. The buffer will be padded so that it can be hashed.
  @param filename name of the file to read and store into buffer.
  @return a ByteBuffer which holds a data array of the padded contents of the
  given file.
*/
ByteBuffer *readFile(const char *filename);

#endif
