/**
  @file byteBuffer.c
  @author Maggie Lin (mclin)
  The file contains a few functions that initialize the ByteBuffer fields, 
  free the memory used to store ByteBuffer and the data array contained within ByteBuffer, 
  read a file stream and store the information within the file in ByteBuffer, 
  and add bytes to the ByteBuffer.
*/
#include "byteBuffer.h"

ByteBuffer *createBuffer()
{
  ByteBuffer *byteBuffer = (ByteBuffer *)malloc(sizeof(ByteBuffer));
  byteBuffer->cap = INITIAL_CAPACITY;
  byteBuffer->len = COUNT_START;
  byteBuffer->data = (byte *)malloc(byteBuffer->cap * sizeof(byte));
  return byteBuffer;
}

void addByte(ByteBuffer *buffer, byte b)
{
  if (buffer->len >= buffer->cap) {
    buffer->cap *= DOUBLE;
    buffer->data = (byte *)realloc(buffer->data, buffer->cap * sizeof(byte));
  }
  buffer->data[buffer->len++] = b;
}

void freeBuffer(ByteBuffer *buffer)
{
  free(buffer->data);
  free(buffer);
}

ByteBuffer *readFile(const char *filename)
{
  FILE *src = fopen(filename, "rb");
  if (src == NULL) {
    return NULL;
  }

  byte current;
  ByteBuffer *buffer = createBuffer();
  while (fscanf(src, "%c", &current) != EOF) {
    addByte(buffer, current);
  }
  fclose(src);
  return buffer;
}
