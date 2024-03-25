/**
  @file ripeMD.h
  @author Maggie Lin (mclin)
  This file contains the struct for HashState which stores five states used during 
  the hash calculation.The file also contains a few function prototypes that initialize 
  the HashState fields, pad buffer so that it's length is a multiple of 64 bytes, print 
  the final hash, and rotate a value left with wraparound. There are also functions prototypes 
  of five different variation of bitwise combinations of states for the hash calculation, 
  function that performs a single hash iteration, function that performs a round of hashing
  (16 itermation of hashing), and a function that performs the whole hash process with a 64
  byte block of data readed from the file.
*/

#ifndef _RIPEMD_H_
#define _RIPEMD_H_

#include "byteBuffer.h"

/** Name for an unsigned 32-bit integer. */
typedef unsigned int longword;

/** Number of bytes in a block. */
#define BLOCK_BYTES 64

/** Number of longwords in a block. */
#define BLOCK_LONGWORDS (BLOCK_BYTES / sizeof(longword))

/** Number of iterations for each round. */
#define RIPE_ITERATIONS 16

/** Number of rounds for RIPEMD hashing. */
#define RIPE_ROUNDS 5

/** First bit at the left side of a longword */
#define LEFT_BIT 0x80000000

/** First bit at the right side bit of a longword */
#define RIGHT_BIT 0x00000001

/** Last byte of a longword */
#define RIGHT_BYTE_MASK 0x000000FF

/** Single bit iteration */
#define SINGLE_BIT_ITER 1

/** Start of RIPEMD padding */
#define PAD_START 0x80

/** RIPEMD padding of zeros to get input buffer a multiple of 64 bytes */
#define PAD_ZERO 0x00

/** Number of bytes in padding that are not zero padding for size */
#define PAD_NOT_ZERO 9

/** Mask to get the last byte of a 64 bit unsigned integer */
#define LONG_BYTE_MASK 0x00000000000000FF

/** Number of bytes in an unsigned long integer */
#define LONG_BYTE 8

/** Number to shift left specified by a single hash iteration */
#define HASH_SHIFT 10

/** Number of bytes in an longword */
#define LONGWORD_BYTE 4



/** Type for a pointer to the bitwise f function used in each round. */
typedef longword (*BitwiseFunction)(longword b, longword c, longword d);

/** Representation for the state of the hash computation.  It's just 4
    unsigned 32-bit integers. Client code can create an instance
    (statically, on the stack or on the heap), but initState() needs
    to initialize it before it can be used. */
typedef struct
{
  /** Hash field A used by RipeMD  */
  longword A;

  /** Hash field B used by RipeMD  */
  longword B;

  /** Hash field C used by RipeMD  */
  longword C;

  /** Hash field D used by RipeMD  */
  longword D;

  /** Hash field E used by RipeMD  */
  longword E;

} HashState;

/**
  Initialize the fields of an HashState instance with five constant values
  for the RIPEMD algorithm.
  @param state the HashState which will contain five constant values to 
  help with hashing.
*/
void initState(HashState *state);

/**
  Pads the given buffer, bring its length up to a multiple of 64 bytes.
  @param buffer the ByteBuffer that needs to be padded for it to be hashable.
*/
void padBuffer(ByteBuffer *buffer);

/**
  Prints out the final hash value stored in the given state as a 160 bit number 
  in hexadecimal.
  @param state the HashState which contains the final hash values.
*/
void printHash(HashState *state);

/**
  Process a block of 64 bytes by converting it to an array of 16 longwords, 
  then performing two separate rounds of hashing, then combine the resulting state 
  values with the original input state values to produce the output state.
  @param state the HashState with the original hash constants.
  @param block the block of 64 bytes to be hashed.
*/
void hashBlock(HashState *state, byte block[BLOCK_BYTES]);

// If we're compiling for test, expose a collection of wrapper
// functions that let us (indirectly) call internal (static) functions
// in this component.

// You shouldn't need to change the following code, and you don't need
// to worry about commenting it.

/**
  Wrapper for bitwiseF0 function for testing.
  @param b B hash constant to be used in bitwiseF0.
  @param c C hash constant to be used in bitwiseF0.
  @param d D hash constant to be used in bitwiseF0.
  @return a longword that represents the result after the three constants 
  are combined.
*/
longword bitwiseF0Wrapper(longword b, longword c, longword d);

/**
  Wrapper for bitwiseF1 function for testing.
  @param b B hash constant to be used in bitwiseF1.
  @param c C hash constant to be used in bitwiseF1.
  @param d D hash constant to be used in bitwiseF1.
  @return a longword that represents the result after the three constants 
  are combined.
*/
longword bitwiseF1Wrapper(longword b, longword c, longword d);

/**
  Wrapper for bitwiseF2 function for testing.
  @param b B hash constant to be used in bitwiseF2.
  @param c C hash constant to be used in bitwiseF2.
  @param d D hash constant to be used in bitwiseF2.
  @return a longword that represents the result after the three constants 
  are combined.
*/
longword bitwiseF2Wrapper(longword b, longword c, longword d);

/**
  Wrapper for bitwiseF3 function for testing.
  @param b B hash constant to be used in bitwiseF3.
  @param c C hash constant to be used in bitwiseF3.
  @param d D hash constant to be used in bitwiseF3.
  @return a longword that represents the result after the three constants 
  are combined.
*/
longword bitwiseF3Wrapper(longword b, longword c, longword d);

/**
  Wrapper for bitwiseF4 function for testing.
  @param b B hash constant to be used in bitwiseF4.
  @param c C hash constant to be used in bitwiseF4.
  @param d D hash constant to be used in bitwiseF4.
  @return a longword that represents the result after the three constants 
  are combined.
*/
longword bitwiseF4Wrapper(longword b, longword c, longword d);

/**
  Wrapper for rotateLeft function for testing.
  @param value value to be shifted left with a wraparound.
  @param s the amount of bits to be shifted over by.
  @return the resulting longword after being shifted left the given 
  amount of shifts with wraparaound.
*/
longword rotateLeftWrapper(longword value, int s);

/**
  Wrapper for a single iteration of the RIPEMD hash algorithm. 
  @param state input state which contains all of the hash constants and 
  also contains the resulting hash constants after the hashing.
  @param datum four bytes from the input array to be used in this iteration of 
  the hashing process.
  @param shift the amount of shifting needed in this iteration of the 
  hashing process.
  @param noise the noise for this iteration of the hashing process.
  @param f the BitwiseFunction to be used for this iteration of the hashing process.
*/
void hashIterationWrapper(HashState *state,
                          longword datum,
                          int shift,
                          longword noise,
                          BitwiseFunction f);

/**
  Wrapper for a round, 16 iterations, of RIPEMD algorithm hashing.
  @param state input state which contains all of the hash constants and 
  also contains the resulting hash constants after the hashing.
  @param data array of longword which represent the content of the file being hashed.
  @param perm array of sequence of data elements to be used during each iteration of 
  the hashing process.
  @param shift array of numbers to shift during each iteration of the hashing process.
  @param noise noise value to be used within this round of hashing.
  @param f bitwises function to be used within this round of hashing.
*/
void hashRoundWrapper(HashState *state,
                      longword data[BLOCK_LONGWORDS],
                      int perm[RIPE_ITERATIONS],
                      int shift[RIPE_ITERATIONS],
                      longword noise,
                      BitwiseFunction f);
#ifdef TESTABLE
#endif

#endif
