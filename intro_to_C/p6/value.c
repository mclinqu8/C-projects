/**
  @file value.c
  @author Maggie Lin (mclin)

  Support for representing different types of values that could be computed
  by our programming language.
*/

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Sequence.

Sequence *makeSequence()
{
  Sequence *seq = (Sequence *)malloc(sizeof(Sequence));
  seq->capacity = INIT_CAP;
  seq->count = 0;
  seq->list = malloc(seq->capacity * sizeof(int));
  seq->ref = 1;
  return seq;
}

void freeSequence(Sequence *seq)
{
  free(seq->list);
  free(seq);
}

void grabSequence(Sequence *seq)
{
  seq->ref += 1;
}

void releaseSequence(Sequence *seq)
{
  seq->ref -= 1;

  if (seq->ref <= 0) {
    assert(seq->ref == 0);
    // Once the reference count hits zero, we can free the sequence memory.
    freeSequence(seq);
  }
}

//////////////////////////////////////////////////////////////////////
// Environment.

/**
  Hidden implementation of the variable.
*/
typedef struct {
  /** Name of the variable. */
  char name[MAX_VAR_NAME + 1];

  /** Value the variable contains. */
  Value val;
} VarRec;

/**
  Hidden implementation of the environment.
*/
struct EnvironmentStruct {
  /** List of variables. */
  VarRec *vlist;

  /** Number of name/value pairs. */
  int len;

  /** Capacity of the name/value list. */
  int capacity;
};

Environment *makeEnvironment()
{
  Environment *env = (Environment *) malloc(sizeof(Environment));
  env->capacity = INIT_CAP;
  env->len = 0;
  env->vlist = (VarRec *) malloc(sizeof(VarRec) * env->capacity);
  return env;
}

Value lookupVariable(Environment *env, char const *name)
{
  // Linear search for a variable name, not too efficient.
  for (int i = 0; i < env->len; i++)
    if (strcmp(env->vlist[ i ].name, name) == 0)
      return env->vlist[i].val;

  // Return zero for uninitialized variables.
  return (Value){IntType, .ival = 0};
}

void setVariable(Environment *env, char const *name, Value value)
{
  int pos = 0;
  while (pos < env->len && strcmp( env->vlist[ pos ].name, name ) != 0)
    pos++;

  if (pos >= env->len) {
    if (env->len >= env->capacity) {
      env->capacity *= DOUBLE;
      env->vlist = (VarRec *) realloc(env->vlist, sizeof(VarRec) * env->capacity);
    }
  }

  // Is this a new variable, or one that already existed?
  if (pos == env->len) {
    pos = env->len++;
    strcpy( env->vlist[pos ].name, name );
  } else if ((env->vlist[pos].val.vtype) == SeqType) {
    releaseSequence(env->vlist[pos].val.sval);
    releaseSequence(env->vlist[pos].val.sval);
  }
  if (value.vtype == SeqType) {
    grabSequence(value.sval);
  }
  env->vlist[pos].val = value;

}

void freeEnvironment(Environment *env)
{
  for (int i = 0; i < env->len; i++) {
    if (env->vlist[i].val.vtype == SeqType) {
      releaseSequence(env->vlist[i].val.sval);
      releaseSequence(env->vlist[i].val.sval);
    }
  }
  free(env->vlist);
  free(env);
}

