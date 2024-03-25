/**
  @file ripeMD.c
  @author Maggie Lin (mclin)
  This file contains a few function that initialize the HashState fields, 
  pad buffer so that it's length is a multiple of 64 bytes, print the final hash, 
  and rotate a value left with wraparound. There are also functions of five different 
  variation of bitwise combinations of states for the hash calculation, function that performs
  a single hash iteration, function that performs a round of hashing(16 itermation of hashing), 
  and a function that performs the whole hash process with a 64 byte block of data readed
  from the file.
*/

#include "ripeMD.h"

void initState(HashState *state)
{
  state->A = 0x67452301;
  state->B = 0xEFCDAB89;
  state->C = 0x98BADCFE;
  state->D = 0x10325476;
  state->E = 0xC3D2E1F0;
}

/**
  First combination of Hash constants B, C, and D with bitwise operations 
  for hashing.
  @param a B hash constant to be used in bitwiseF0.
  @param b C hash constant to be used in bitwiseF0.
  @param c D hash constant to be used in bitwiseF0.
  @return a longword that represents the result after the three constants 
  are combined.
*/
static longword bitwiseF0(longword a, longword b, longword c)
{
  return a ^ b ^ c;
}

/**
  Second combination of Hash constants B, C, and D with bitwise operations 
  for hashing.
  @param a B hash constant to be used in bitwiseF1.
  @param b C hash constant to be used in bitwiseF1.
  @param c D hash constant to be used in bitwiseF1.
  @return a longword that represents the result after the three constants 
  are combined.
*/
static longword bitwiseF1(longword a, longword b, longword c)
{
  return (a & b) | ((~a) & c);
}

/**
  Third combination of Hash constants B, C, and D with bitwise operations 
  for hashing.
  @param a B hash constant to be used in bitwiseF2.
  @param b C hash constant to be used in bitwiseF2.
  @param c D hash constant to be used in bitwiseF2.
  @return a longword that represents the result after the three constants 
  are combined.
*/
static longword bitwiseF2(longword a, longword b, longword c)
{
  return (a | (~b)) ^ c;
}

/**
  Fourth combination of Hash constants B, C, and D with bitwise operations 
  for hashing.
  @param a B hash constant to be used in bitwiseF3.
  @param b C hash constant to be used in bitwiseF3.
  @param c D hash constant to be used in bitwiseF3.
  @return a longword that represents the result after the three constants 
  are combined.
*/
static longword bitwiseF3(longword a, longword b, longword c)
{
  return (a & c) | (b & (~c));
}

/**
  Fifth combination of Hash constants B, C, and D with bitwise operations 
  for hashing.
  @param a B hash constant to be used in bitwiseF4.
  @param b C hash constant to be used in bitwiseF4.
  @param c D hash constant to be used in bitwiseF4.
  @return a longword that represents the result after the three constants 
  are combined.
*/
static longword bitwiseF4(longword a, longword b, longword c)
{
  return a ^ (b | (~c));
}

/**
  Rotates the given value to the left by the given amount of bits with 
  wraparound so no values are lost.
  @param value value to be shifted left with a wraparound.
  @param s the amount of bits to be shifted over by.
  @return the resulting longword after being shifted left the given 
  amount of shifts with wraparaound.
*/
static longword rotateLeft(longword value, int s)
{
  longword mask = LEFT_BIT;
  longword mask2 = RIGHT_BIT;
  longword result = value;
  longword bit;
  for (int i = COUNT_START; i < s; i++) {
    bit = result & mask;
    // If the left bit is a 1
    if (bit == mask) {
      result = result << SINGLE_BIT_ITER;
      result = result | mask2;
    } else {
      result = result << SINGLE_BIT_ITER;
    }
  }
  return result;
}

void padBuffer(ByteBuffer *buffer)
{
  byte padStart = PAD_START;
  byte padding = PAD_ZERO;
  unsigned long mask = LONG_BYTE_MASK;
  unsigned long originalLen = BBITS * buffer->len;
  byte current;
  int space = BLOCK_BYTES - (buffer->len % BLOCK_BYTES);
  if (space == COUNT_START) {
    space = BLOCK_BYTES;
  }
  space = space - PAD_NOT_ZERO;
  addByte(buffer, padStart);
  for (int i = COUNT_START; i < space; i++) {
    addByte(buffer, padding);
  }
  for (int j = COUNT_START; j < LONG_BYTE; j++) {
    current = (originalLen >> j * BBITS) & mask;
    addByte(buffer, current);
  }
}

/**
  A single iteration of the RIPEMD hash algorithm. 
  @param state input state which contains all of the hash constants and 
  also contains the resulting hash constants after the hashing.
  @param datum four bytes from the input array to be used in this iteration of 
  the hashing process.
  @param shift the amount of shifting needed in this iteration of the 
  hashing process.
  @param noise the noise for this iteration of the hashing process.
  @param f the BitwiseFunction to be used for this iteration of the hashing process.
*/
static void hashIteration(HashState *state, longword datum, int shift, longword noise, BitwiseFunction f)
{
  longword result;
  result = state->A + f(state->B, state->C, state->D);
  result = result + datum;
  result = result + noise;
  result = rotateLeft(result, shift);
  state->C = rotateLeft(state->C, HASH_SHIFT);
  result = result + state->E;
  state->A = state->E;
  state->E = state->D;
  state->D = state->C;
  state->C = state->B;
  state->B = result;
}

/**
  Implements a round, 16 iterations, of RIPEMD algorithm hashing.
  @param state input state which contains all of the hash constants and 
  also contains the resulting hash constants after the hashing.
  @param data array of longword which represent the content of the file being hashed.
  @param perm array of sequence of data elements to be used during each iteration of 
  the hashing process.
  @param shift array of numbers to shift during each iteration of the hashing process.
  @param noise noise value to be used within this round of hashing.
  @param f bitwises function to be used within this round of hashing.
*/
static void hashRound(HashState *state, longword data[BLOCK_LONGWORDS], int perm[RIPE_ITERATIONS],
                      int shift[RIPE_ITERATIONS], longword noise, BitwiseFunction f)
{
  for (int i = COUNT_START; i < RIPE_ITERATIONS; i++) {
    hashIteration(state, data[perm[i]], shift[i], noise, f);
  }
}

void hashBlock(HashState *state, byte block[BLOCK_BYTES])
{

  HashState left;
  HashState right;
  left.A = state->A;
  right.A = state->A;
  left.B = state->B;
  right.B = state->B;
  left.C = state->C;
  right.C = state->C;
  left.D = state->D;
  right.D = state->D;
  left.E = state->E;
  right.E = state->E;

  int leftPerm[RIPE_ROUNDS][RIPE_ITERATIONS] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8},
    {3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12},
    {1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2},
    {4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13}};

  int rightPerm[RIPE_ROUNDS][RIPE_ITERATIONS] = {
    {5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12},
    {6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2},
    {15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13},
    {8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14},
    {12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11}};

  int leftShift[RIPE_ROUNDS][RIPE_ITERATIONS] = {
    {11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8},
    {7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12},
    {11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5},
    {11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12},
    {9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6}};

  int rightShift[RIPE_ROUNDS][RIPE_ITERATIONS] = {
    {8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6},
    {9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11},
    {9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5},
    {15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8},
    {8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11}};

  longword leftNoise[RIPE_ROUNDS] = {0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E};

  longword rightNoise[RIPE_ROUNDS] = {0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000};

  BitwiseFunction leftFunc[RIPE_ROUNDS] = {&bitwiseF0, &bitwiseF1, &bitwiseF2, &bitwiseF3, &bitwiseF4};
  BitwiseFunction rightFunc[RIPE_ROUNDS] = {&bitwiseF4, &bitwiseF3, &bitwiseF2, &bitwiseF1, &bitwiseF0};

  for (int k = COUNT_START; k < RIPE_ROUNDS; k++) {
    hashRound(&left, (longword *)block, leftPerm[k], leftShift[k], leftNoise[k], leftFunc[k]);
    hashRound(&right, (longword *)block, rightPerm[k], rightShift[k], rightNoise[k], rightFunc[k]);
  }

  longword resultA = left.C + state->B + right.D;
  longword resultB = left.D + state->C + right.E;
  longword resultC = left.E + state->D + right.A;
  longword resultD = left.A + state->E + right.B;
  longword resultE = left.B + state->A + right.C;

  state->A = resultA;
  state->B = resultB;
  state->C = resultC;
  state->D = resultD;
  state->E = resultE;
}

void printHash(HashState *state)
{
  longword mask = RIGHT_BYTE_MASK;
  longword current;
  for (int j = COUNT_START; j < LONGWORD_BYTE; j++) {
    current = (state->A >> j * BBITS) & mask;
    printf("%02x", current);
  }
  for (int j = COUNT_START; j < LONGWORD_BYTE; j++) {
    current = (state->B >> j * BBITS) & mask;
    printf("%02x", current);
  }
  for (int j = COUNT_START; j < LONGWORD_BYTE; j++) {
    current = (state->C >> j * BBITS) & mask;
    printf("%02x", current);
  }
  for (int j = COUNT_START; j < LONGWORD_BYTE; j++) {
    current = (state->D >> j * BBITS) & mask;
    printf("%02x", current);
  }
  for (int j = COUNT_START; j < LONGWORD_BYTE; j++) {
    current = (state->E >> j * BBITS) & mask;
    printf("%02x", current);
  }
  printf("\n");
}

// Put the following at the end of your implementation file.
// If we're compiling for unit tests, create wrappers for the otherwise
// private functions we'd like to be able to test.

longword bitwiseF0Wrapper(longword b, longword c, longword d)
{
  return bitwiseF0(b, c, d);
}

longword bitwiseF1Wrapper(longword b, longword c, longword d)
{
  return bitwiseF1(b, c, d);
}

longword bitwiseF2Wrapper(longword b, longword c, longword d)
{
  return bitwiseF2(b, c, d);
}

longword bitwiseF3Wrapper(longword b, longword c, longword d)
{
  return bitwiseF3(b, c, d);
}

longword bitwiseF4Wrapper(longword b, longword c, longword d)
{
  return bitwiseF4(b, c, d);
}

longword rotateLeftWrapper(longword value, int s)
{
  return rotateLeft(value, s);
}

void hashIterationWrapper(HashState *state,
                          longword datum,
                          int shift,
                          longword noise,
                          BitwiseFunction f)
{
  hashIteration(state, datum, shift, noise, f);
}

void hashRoundWrapper(HashState *state,
                      longword *block,
                      int perm[RIPE_ITERATIONS],
                      int shift[RIPE_ITERATIONS],
                      longword noise,
                      BitwiseFunction f)
{
  hashRound(state, block, perm, shift, noise, f);
}
#ifdef TESTABLE
#endif
