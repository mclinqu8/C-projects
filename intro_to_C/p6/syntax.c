/**
  @file syntax.c
  @author Maggie Lin
  This provides the implementation of the Expr and Stmt abstract classes, 
  along with most of the concrete subclasses representing different parts of 
  the parse tree for the program currently runs. This allows for the recognization 
  of the pseudocode and interpret it to valid commands of C.
*/

#include "syntax.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Error-reporting functions

/** Report an error for a program with bad types, then exit. */
static int reportTypeMismatch()
{
  fprintf(stderr, "Type mismatch\n");
  exit(EXIT_FAILURE);
}

/**
  Require a given value to be an IntType value.  Exit with an error
  message if not.
  @param v value to check, passed by address.
*/
static void requireIntType(Value const *v)
{
  if (v->vtype != IntType)
    reportTypeMismatch();
}

/**
  Require a given value to be an SeqType value.  Exit with an error
  message if not.
  @param v value to check, passed by address.
*/
static void requireSeqType(Value const *v)
{
  if (v->vtype != SeqType)
    reportTypeMismatch();
}

//////////////////////////////////////////////////////////////////////
// LiteralInt

/**
  Representation for a LiteralInt expression, a subclass of Expr that
  evaluates to a constant value.
*/
typedef struct {
  /** An evaluate function for a LiteralInt. */
  Value (*eval)(Expr *expr, Environment *env);
  /** A destroy function for a LiteralInt. */
  void (*destroy)(Expr *expr);

  /** Integer value this expression evaluates to. */
  int val;
} LiteralInt;

/**
  Implementation of eval for LiteralInt expressions.
  @param expr Expression to evaluate into a LiteralInt.
  @param env The Environment.
  @return Value with the evaluated integer.
*/
static Value evalLiteralInt(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a LiteralInt.
  LiteralInt *this = (LiteralInt *)expr;

  // Return an int value containing a copy of the value we represent.
  return (Value){IntType, .ival = this->val};
}

/**
  Implementation of destroy for LiteralInt expressions. Frees the space
  allocated to a LiteralInt expression.
  @param expr Expression to destroy.
*/
static void destroyLiteralInt(Expr *expr)
{
  // This object is just one block of memory.  We can free it without
  // even having to type-cast its pointer.
  free(expr);
}

Expr *makeLiteralInt(int val)
{
  // Allocate space for the LiteralInt object
  LiteralInt *this = (LiteralInt *) malloc(sizeof(LiteralInt));

  // Remember the pointers to functions for evaluating and destroying ourself.
  this->eval = evalLiteralInt;
  this->destroy = destroyLiteralInt;

  // Remember the integer value we contain.
  this->val = val;

  // Return the result, as an instance of the Expr superclass.
  return (Expr *) this;
}


//////////////////////////////////////////////////////////////////////
// SeqExpr

/**
  Representation for a Sequence expression, a subclass of Expr that
  evaluates to a Sequence.
*/
typedef struct {
  /** An evaluate function for a SeqExpr. */
  Value (*eval)(Expr *expr, Environment *env);
  /** A destroy function for a SeqExpr. */
  void (*destroy)(Expr *expr);

  /** Number of expressions to be evaluated and stored in the sequence. */
  int count;

  /** List of expressions to be evaluated and stored in the sequence. */
  Expr **exprs;
} SeqExpr;

/**
  Implementation of eval for SeqExpr expressions
  @param expr Expression to evaluate into a LiteralInt.
  @param env The Environment.
  @return Value with the evaluated integer.
*/
static Value evalSequenceInitializer(Expr *expr, Environment *env)
{

  // If this function gets called, expr must really be a SeqExpr.
  SeqExpr *this = (SeqExpr *)expr;
  Sequence *seq = makeSequence();
  // grabSequence(seq);
  if (this->count >= seq->capacity) {
    seq->capacity = this->count;
    seq->list = (int * ) realloc(seq->list, seq->capacity * sizeof(int));
  }
  for (int i = 0; i < this->count; i++) {
    seq->list[i] = this->exprs[i]->eval(this->exprs[i], env).ival;
    seq->count++;
  }
  // Return an int value containing a copy of the value we represent.
  return (Value){SeqType, .sval = seq};
}

/**
  Implementation of destroy for SeqExpr expressions. Frees the space
  allocated to a SeqExpr expression.
  @param expr Expression to destroy.
*/
static void destroySequenceInitializer(Expr *expr)
{
  SeqExpr *this = (SeqExpr *)expr;
  for (int i = 0; i < this->count; i++) {
    this->exprs[i]->destroy(this->exprs[i]);
  }

  free(this->exprs);
  free(this);
}

Expr *makeSequenceInitializer(int len, Expr *eList[])
{
  // Allocate space for the LiteralSeq object
  SeqExpr *this = (SeqExpr *) malloc(sizeof(SeqExpr));
  // Sequence *seq = makeSequence();

  // this->seq = makeSequence();
  this->count = len;
  this->exprs = eList;

  // Remember the pointers to functions for evaluating and destroying ourself.
  this->eval = evalSequenceInitializer;
  this->destroy = destroySequenceInitializer;

  // Return the result, as an instance of the Expr superclass.
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// SimpleExpr Struct

/** 
  Representation for an expression with either one or two
  sub-expressionts.  With the right eval funciton, this struct should
  be able to help implement any expression with either one or two
  sub-expressiosn.
*/
typedef struct {
  Value (*eval)(Expr *expr, Environment *env);
  void (*destroy)(Expr *oper);

  /** The first sub-expression */
  Expr *expr1;
  
  /** The second sub-expression, or NULL if it's not needed. */
  Expr *expr2;
} SimpleExpr;

/** 
  General-purpose function for freeing an expression represented by
  SimpleExpr.  It frees the two sub-expressions, then frees the strucct
  itself.
*/
static void destroySimpleExpr(Expr *expr)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Free the first sub-expression.
  this->expr1->destroy(this->expr1);

  // Free the second one, if it exists.
  if (this->expr2)
    this->expr2->destroy(this->expr2);

  // Then the SimpleExpr struct itself.
  free(this);
}

/** 
  Helper funciton to construct a SimpleExpr representation and fill
  in the fields.
  @param first sub-expression in the expression.
  @param second sub-expression in the expression, or null if it only
  has one sub-expression.
  @param eval function implementing the eval mehod for this expression.
  @return new expression, as a poiner to Expr.
*/
static Expr *buildSimpleExpr(Expr *expr1, Expr *expr2,
                              Value (*eval)(Expr *, Environment *))
{
  // Allocate space for a new SimpleExpr and fill in the pointer for
  // its destroy function.
  SimpleExpr *this = (SimpleExpr *) malloc(sizeof(SimpleExpr));
  this->destroy = destroySimpleExpr;

  // Fill in the two parameters and the eval funciton.
  this->eval = eval;
  this->expr1 = expr1;
  this->expr2 = expr2;

  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Integer addition

/** 
  Implementation of the eval function for integer addition.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent the value after value one was added to value two.
*/
static Value evalAdd(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);

  // Make sure the operands are both integers.
  requireIntType(&v1);
  requireIntType(&v2);

  // printf("evalAdd\n%d\n%d\n",  v1.ival,  v2.ival);
  // Return the sum of the two expression values.
  return (Value){IntType, .ival = v1.ival + v2.ival};
}

Expr *makeAdd(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for addition
  return buildSimpleExpr(left, right, evalAdd);
}

//////////////////////////////////////////////////////////////////////
// Integer subtracton

/** 
  Implementation of the eval function for integer subtraction.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent the value after value one was subtracted by value two.
*/
static Value evalSub(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);

  // Make sure the operands are both integers.
  requireIntType(&v1);
  requireIntType(&v2);

  // Return the difference of the two expression values.
  return (Value){IntType, .ival = v1.ival - v2.ival};
}

Expr *makeSub(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for subtraction.
  return buildSimpleExpr(left, right, evalSub);
}

//////////////////////////////////////////////////////////////////////
// Integer multiplication

/** 
  Implementation of the eval function for integer multiplication. 
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent the value after value one was multiplied by value two.
*/
static Value evalMul(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);

  // Make sure the operands are both integers.
  requireIntType(&v1);
  requireIntType(&v2);

  // Return the product of the two expression.
  return (Value){IntType, .ival = v1.ival * v2.ival};
}

Expr *makeMul(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for multiplication.
  return buildSimpleExpr(left, right, evalMul);
}

//////////////////////////////////////////////////////////////////////
// Integer division

/** 
  Implementation of the eval function for integer division.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent the value after value one was divided by value two.
*/
static Value evalDiv(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);

  // Make sure the operands are both integers.
  requireIntType(&v1);
  requireIntType(&v2);

  // Catch it if we try to divide by zero.
  if (v2.ival == 0) {
    fprintf(stderr, "Divide by zero\n");
    exit(EXIT_FAILURE);
  }

  // Return the quotient of the two expression.
  return (Value){IntType, .ival = v1.ival / v2.ival};
}

Expr *makeDiv(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for division.
  return buildSimpleExpr(left, right, evalDiv);
}

//////////////////////////////////////////////////////////////////////
// Logical and

/** 
  Implementation of eval for the and operator.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent whether value one and value two is true.
*/
static Value evalAnd(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate the left operand; return immediately if it's false.
  Value v1 = this->expr1->eval(this->expr1, env);
  requireIntType(&v1);
  if (v1.ival == 0)
    return v1;
  
  // Evaluate the right operand.
  Value v2 = this->expr2->eval(this->expr2, env);
  requireIntType(&v2);

  // Return true if the right-hand operand is true.
  return v2;
}

Expr *makeAnd(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for the logical and.
  return buildSimpleExpr(left, right, evalAnd);
}

//////////////////////////////////////////////////////////////////////
// Logical or

/** 
  Implementation of eval for the or operator.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent whether value one or value two is true.
*/
static Value evalOr(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate the left operand; return immediately if it's true.
  Value v1 = this->expr1->eval(this->expr1, env);
  requireIntType(&v1);
  if (v1.ival)
    return v1;
  
  // Evaluate the right operand
  Value v2 = this->expr2->eval(this->expr2, env);
  requireIntType(&v2);

  // Return true if the right-hand operand is true.
  return v2;
}

Expr *makeOr(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for the logical or
  return buildSimpleExpr(left, right, evalOr);
}

//////////////////////////////////////////////////////////////////////
// Less-than comparison

/** 
  Implementation of eval for the less than operator.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent whether value one was less than value two.
*/
static Value evalLess(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);

  // Make sure the operands are both the same type.
  if (v1.vtype != v2.vtype) {
    if (v1.vtype == SeqType) {
      releaseSequence(v1.sval);
    }
    if (v2.vtype == SeqType) {
      releaseSequence(v2.sval);
    }
    reportTypeMismatch();
  }

  if (v1.vtype == IntType) {
    // Is v1 less than v2
    return (Value){IntType, .ival = v1.ival < v2.ival ? true : false};
  } else {
    // Replace with code to compare sequences.
    if (v1.sval->count > v2.sval->count) {
      releaseSequence(v1.sval);
      releaseSequence(v2.sval);
      return (Value){IntType, .ival = 0};
    } 
    int first;
    int second;
    for (int i = 0; i < v1.sval->count; i++) {
      first = v1.sval->list[i];
      second = v2.sval->list[i];
      if (first < second) {
        releaseSequence(v1.sval);
        releaseSequence(v2.sval);
        return (Value){IntType, .ival = 1};
      }
      if (first > second) {
        releaseSequence(v1.sval);
        releaseSequence(v2.sval);
        return (Value){IntType, .ival = 0};
      }
    }
    if (v1.sval->count < v2.sval->count) {
      releaseSequence(v1.sval);
      releaseSequence(v2.sval);
      return (Value){IntType, .ival = 1};
    } 
    releaseSequence(v1.sval);
    releaseSequence(v2.sval);
    return (Value){IntType, .ival = 0};
  }
}

Expr *makeLess(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for the less-than
  // comparison.
  return buildSimpleExpr(left, right, evalLess);
}

//////////////////////////////////////////////////////////////////////
// Equality comparison

/** 
  Eval function for an equality test.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value that represent of the two are equal to each other.
*/
static Value evalEquals(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);

  // Make sure the same type.
  if (v1.vtype == IntType && v2.vtype == IntType) {
    return (Value){IntType, .ival = (v1.ival == v2.ival)};
  } else {
    // Replace with code to permit sequence-sequence comparison.
    // A sequence can also be compared to an int, but they should
    // never be considered equal.
    if (v1.vtype != SeqType) {
      if (v2.vtype == SeqType) {
        releaseSequence(v2.sval);
      }
      return (Value){ IntType, .ival = 0 };
    } 
    if (v2.vtype != SeqType) {
      if (v1.vtype == SeqType) {
        releaseSequence(v1.sval);
      }
      return (Value){IntType, .ival = 0};
    } 
    if (v1.sval->count != v2.sval->count) {
      releaseSequence(v1.sval);
      releaseSequence(v2.sval);
      return (Value){IntType, .ival = 0};
    } 
    int first;
    int second;
    for (int i = 0; i < v1.sval->count; i++) {
      first = v1.sval->list[i];
      second = v2.sval->list[i];
      if (first != second) {
        releaseSequence(v1.sval);
        releaseSequence(v2.sval);
        return (Value){IntType, .ival = 0};
      }
    }
    releaseSequence(v1.sval);
    releaseSequence(v2.sval);
    return (Value){IntType, .ival = 1};
  }
  return (Value){IntType, .ival = 0};
}

Expr *makeEquals(Expr *left, Expr *right)
{
  // Use the convenience function to build a SimpleExpr for the equals test.
  return buildSimpleExpr(left, right, evalEquals);
}

//////////////////////////////////////////////////////////////////////
// Length function

/** 
  Implementation of eval for Length.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return length of the given sequence.
*/
static Value evalLen(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our operand.
  Value v1 = this->expr1->eval(this->expr1, env);
  requireSeqType(&v1);
  
  int value = v1.sval->count;
  releaseSequence(v1.sval);
  return (Value){IntType, .ival = value};
}

Expr *makeLenExpr(Expr *expr)
{
  // Use the convenience function to build a SimpleExpr for Len
  return buildSimpleExpr(expr, NULL, evalLen);
}

//////////////////////////////////////////////////////////////////////
// Retrieve element at index

/** 
  Implementation of eval for Index.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value at the given index name.
*/
static Value evalIndex(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);
  requireIntType(&v2);
  if (v2.ival > v1.sval->count - 1) {
    fprintf(stderr, "Index out of bounds\n");
    exit(1);
  }
  int value = v1.sval->list[v2.ival];
  releaseSequence(v1.sval);
  return (Value){IntType, .ival = value};
}

Expr *makeSequenceIndex(Expr *aexpr, Expr *iexpr)
{
  // Use the convenience function to build a SimpleExpr for index
  return buildSimpleExpr(aexpr, iexpr, evalIndex);
}


//////////////////////////////////////////////////////////////////////
// Variable in an expression

/** 
  Representation for an expression representing an occurrence of a
  variable, subclass of Expr.
*/
typedef struct {
  Value (*eval)(Expr *expr, Environment *env);
  void (*destroy)(Expr *expr);

  /** Name of the variable. */
  char name[MAX_VAR_NAME + 1];
} VariableExpr;

/** 
  Implementation of eval for Variable.
  @param expr Expression to evaluate.
  @param env The Environment.
  @return value with given variable name.
*/
static Value evalVariable(Expr *expr, Environment *env)
{
  // If this function gets called, expr must really be a VariableExpr
  VariableExpr *this = (VariableExpr *) expr;

  // Get the value of this variable.
  Value val = lookupVariable(env, this->name);
  if (val.vtype == SeqType) {
    grabSequence(val.sval);
  }
  return val;
}

/** 
  Implementation of destroy for Variable.
  @param expr Expression to destroy.
*/
static void destroyVariable(Expr *expr)
{
  free(expr);
}

Expr *makeVariable(char const *name)
{
  // Allocate space for the Variable statement, and fill in its function
  // pointers and a copy of the variable name.
  VariableExpr *this = (VariableExpr *) malloc(sizeof(VariableExpr));
  this->eval = evalVariable;
  this->destroy = destroyVariable;
  strcpy(this->name, name);

  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// SimpleStmt Struct

/** 
  Generic representation for a statement that contains one or two
  expressions.  With different execute methods, this same struct
  can be used to represent print and push statements.
*/
typedef struct {
  void (*execute)(Stmt *stmt, Environment *env);
  void (*destroy)(Stmt *stmt);

  /** First (or only) expression used by this statement. */
  Expr *expr1;
  /** Second expression used by this statement, or null */
  Expr *expr2;
} SimpleStmt;

/** Generic destroy function for SimpleStmt, with either one
    or two sub-expressions. */

/** 
  Generic destroy function for SimpleStmt, with either one
  or two sub-expressions.
  @param stmt Statement to destroy.
*/
static void destroySimpleStmt(Stmt *stmt)
{
  // If this function gets called, stmt must really be a SimpleStmt.
  SimpleStmt *this = (SimpleStmt *)stmt;

  // Free our subexpression then the SimpleStmt object itself.
  this->expr1->destroy(this->expr1);
  if (this->expr2)
    this->expr2->destroy(this->expr2);
  free(this);
}

//////////////////////////////////////////////////////////////////////
// Print Statement

/** 
  Implementation of execute for a print statement.
  @param stmt Statement object for Print.
  @param env the Enviroment.
*/
static void executePrint(Stmt *stmt, Environment *env)
{
  // If this function gets called, stmt must really be a SimpleStmt.
  SimpleStmt *this = (SimpleStmt *)stmt;

  // Evaluate our argument.
  Value v = this->expr1->eval(this->expr1, env);

  // Print the value of our expression appropriately, based on its type.
  if (v.vtype == IntType) {
    printf("%d", v.ival);
  } else {
    // Replace with code to permit print a sequence as a string of
    // ASCII character codes.
    char seq[v.sval->count + 1];
    for (int i = 0; i < v.sval->count; i++) {
      seq[i] = v.sval->list[i];
      printf("%c", seq[i]);
    }
    releaseSequence(v.sval);
  }
}

Stmt *makePrint(Expr *expr)
{
  // Allocate space for the SimpleStmt object
  SimpleStmt *this = (SimpleStmt *) malloc(sizeof(SimpleStmt));

  // Remember the pointers to execute and destroy this statement.
  this->execute = executePrint;
  this->destroy = destroySimpleStmt;

  // Remember the expression for the thing we're supposed to print.
  this->expr1 = expr;
  this->expr2 = NULL;

  // Return the SimpleStmt object, as an instance of the Stmt interface.
  return (Stmt *) this;
}

//////////////////////////////////////////////////////////////////////
// Compound Statement

/** 
  Representation for a compound statement, derived from Stmt.
*/
typedef struct {
  void (*execute)(Stmt *stmt, Environment *env);
  void (*destroy)(Stmt *stmt);

  /** Number of statements in the compound. */
  int len;
  
  /** List of statements in the compound. */
  Stmt **stmtList;
} CompoundStmt;

/** 
  Implementation of execute for CompountStmt.
  @param stmt Statement object for CompountStmt.
  @param env the Enviroment.
*/
static void executeCompound(Stmt *stmt, Environment *env)
{
  // If this function gets called, stmt must really be a CompoundStmt.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Execute the sequence of statements in this compound
  for (int i = 0; i < this->len; i++)
    this->stmtList[i]->execute(this->stmtList[ i ], env);
}

/** 
  Implementation of destroy for CompountStmt.
  @param stmt Statement object for CompountStmt.
*/
static void destroyCompound(Stmt *stmt)
{
  // If this function gets called, stmt must really be a CompoundStmt.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Free the list of statements inside this compond.
  for (int i = 0; i < this->len; i++)
    this->stmtList[i]->destroy( this->stmtList[i]);

  // Then, free the array of pointers and the compund statement itself.
  free(this->stmtList);
  free(this);
}

Stmt *makeCompound(int len, Stmt **stmtList)
{
  // Allocate space for the CompoundStmt object
  CompoundStmt *this = (CompoundStmt *) malloc(sizeof(CompoundStmt));

  // Remember the pointers to execute and destroy this statement.
  this->execute = executeCompound;
  this->destroy = destroyCompound;

  // Remember the list of statements in the compound.
  this->len = len;
  this->stmtList = stmtList;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// ConditioanlStatement (for while/if)

/** 
  Representation for either a while or if statement, subclass of Stmt
*/
typedef struct {
  void (*execute)(Stmt *stmt, Environment *env);
  void (*destroy)(Stmt *stmt);

  /** Condition to be checked before running the body. */
  Expr *cond;

  /** Body to execute if / while cond is true. */
  Stmt *body;
} ConditionalStmt;

/** 
  Implementation of destroy for either while of if statements.
*/
static void destroyConditional(Stmt *stmt)
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Destroy the condition expression and the statement in the body.
  this->cond->destroy(this->cond);
  this->body->destroy(this->body);

  // Then, free the ConditionalStmt struct.
  free(this);
}

///////////////////////////////////////////////////////////////////////
// If statement

/** 
  Implementation of th execute function for an if statement. 
  @param stmt Statement object for if.
  @param env the Enviroment.
*/
static void executeIf(Stmt *stmt, Environment *env)
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Evaluate our operand and see if it's true.
  Value result = this->cond->eval(this->cond, env);
  requireIntType(&result);

  // Execute the body if the condition evaluated to true.
  if (result.ival)
    this->body->execute(this->body, env);
}

Stmt *makeIf(Expr *cond, Stmt *body)
{
  // Allocate an instance of ConditionalStmt
  ConditionalStmt *this =
    (ConditionalStmt *) malloc(sizeof(ConditionalStmt));

  // Functions to execute and destroy an if statement.
  this->execute = executeIf;
  this->destroy = destroyConditional;

  // Fill in the condition and the body of the if.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// While statement

/** 
  Implementation of th execute function for a while statement.
  @param stmt Statement object for while.
  @param env the Enviroment.
*/
static void executeWhile(Stmt *stmt, Environment *env)
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Evaluate our condition and see if it's true.
  Value result = this->cond->eval(this->cond, env);
  requireIntType(&result);
  
  // Execute the body while the condition evaluates to true.
  while (result.ival) {
    this->body->execute(this->body, env);
    
    // Get the value of the condition for the next iteration.
    result = this->cond->eval(this->cond, env);
    requireIntType(&result);
  }
}

Stmt *makeWhile(Expr *cond, Stmt *body)
{
  // Allocate an instance of ConditionalStmt
  ConditionalStmt *this =
    (ConditionalStmt *) malloc(sizeof(ConditionalStmt));

  // Functions to execute and destroy a while statement.
  this->execute = executeWhile;
  this->destroy = destroyConditional;

  // Fill in the condition and the body of the while.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// Assignment statement

/** 
  Representation of an assignment statement, a subclass of
  Stmt. This representation should be suitable for assigning to a
  variable or an element of a sequence.
*/
typedef struct {
  void (*execute)(Stmt *stmt, Environment *env);
  void (*destroy)(Stmt *stmt);

  /** Name of the variable we're assigning to. */
  char name[MAX_VAR_NAME + 1];
  
  /** If we're assigning to an element of a sequence, this is the index
      expression. Otherwise, it's zero. */
  Expr *iexpr;

  /** Expression for the right-hand side of the assignment (the source). */
  Expr *expr;
} AssignmentStmt;

/** 
  Implementation of destroy for assignment Statements.
*/
static void destroyAssignment(Stmt *stmt)
{
  AssignmentStmt *this = (AssignmentStmt *)stmt;

  // Destroy the source expression and the index (if there is one)
  this->expr->destroy(this->expr);
  if (this->iexpr)
    this->iexpr->destroy(this->iexpr);
  free(this);
}

/** 
  Implementation of execute for assignment Statements.
  @param stmt Statement object for assignment.
  @param env the Enviroment.
*/
static void executeAssignment(Stmt *stmt, Environment *env)
{
  // If we get to this function, stmt must be an AssignmentStmt.
  AssignmentStmt *this = (AssignmentStmt *) stmt;

  // Evaluate the right-hand side of the equals.
  Value result = this->expr->eval(this->expr, env);
  
  if (this->iexpr) {
    // Replace with code to permit assigning to a sequence element.
    Value seqval = lookupVariable(env, this->name);
    Value idx = this->iexpr->eval(this->iexpr, env);
    if (idx.ival > seqval.sval->count - 1) {
      fprintf(stderr, "Index out of bounds\n");
      exit(1);
    }
    seqval.sval->list[idx.ival] = result.ival;
  } else {
    setVariable(env, this->name, result);
  }
}

Stmt *makeAssignment(char const *name, Expr *iexpr, Expr *expr)
{

  // Allocate the AssignmentStmt representations.
  AssignmentStmt *this =
    (AssignmentStmt *) malloc(sizeof(AssignmentStmt));

  // Fill in functions to execute or destory this statement.
  this->execute = executeAssignment;
  this->destroy = destroyAssignment;

  // Get a copy of the destination variable name, the source
  // expression and the sequence index (if it's non-null).
  strcpy(this->name, name);
  this->iexpr = iexpr;
  this->expr = expr;

  // Return this object, as an instance of Stmt.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// Push statement

/** 
  Representation of an push statement, a subclass of
  Stmt.
*/
typedef struct {
  void (*execute)(Stmt *stmt, Environment *env);
  void (*destroy)(Stmt *stmt);
  
  /** Expression for the sequence */
  Expr *seqExpr;

  /** Expression for the val that will be pushed */
  Expr *valExpr;
} PushStmt;

/** 
  Implementation of destroy for push Statements.
  @param stmt Statement object for push.
*/
static void destroyPush(Stmt *stmt)
{
  PushStmt *this = (PushStmt *)stmt;

  // Destroy the source expression and the index (if there is one)
  this->seqExpr->destroy(this->seqExpr);
  this->valExpr->destroy(this->valExpr);
  free( this );
}

/** Implementation of execute for assignment Statements. */

/** 
  Implementation of execute for push Statements.
  @param stmt Statement object for push.
  @param env the Enviroment.
*/
static void executePush(Stmt *stmt, Environment *env)
{
  // If we get to this function, stmt must be an PushStmt.
  PushStmt *this = (PushStmt *) stmt;

  Value seqResult = this->seqExpr->eval(this->seqExpr, env);
  Value valResult = this->valExpr->eval(this->valExpr, env);
  requireSeqType(&seqResult);
  requireIntType(&valResult);
  // assign value to the last element of the sequence
  //right here 
  if (seqResult.sval->count >= seqResult.sval->capacity) {
    seqResult.sval->capacity *= DOUBLE;
    seqResult.sval->list = (int * ) realloc(seqResult.sval->list, seqResult.sval->capacity * sizeof(int));
  }
  seqResult.sval->list[seqResult.sval->count++] = valResult.ival;
  releaseSequence(seqResult.sval);
}

Stmt *makePushStmt(Expr *sexpr, Expr *vexpr)
{
  // Allocate the PushStmt representations.
  PushStmt *this =
    (PushStmt *) malloc(sizeof(PushStmt));

  // Fill in functions to execute or destory this statement.
  this->execute = executePush;
  this->destroy = destroyPush;

  this->seqExpr = sexpr;
  this->valExpr = vexpr;

  // Return this object, as an instance of Stmt.
  return (Stmt *) this;
}
