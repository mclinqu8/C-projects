/** 
  @file polish.c
  @author Maggie Lin (mclin)
  This program is responsible for reading and evaluating the expressions 
  and printing the results of the polish expressions.
*/

#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "bounds.h"

/** ASCII for new line. */
#define NEW_LINE 10

/** 
  Recursive function that is responsible for reading a section of an expression
  in polish notation, calculating the result of the expression and returning it.
  @return the result of a section of polish notation.
*/
static long parseExpression() {
  signed long ch = getchar();
  signed long exp1;
  signed long exp2;
  

  while(ch == ' ' || ch == '\n') {
    ch = getchar();
  }

  if (ch == EOF) {
    return ch;
  }

  if (ch != '+' && ch != '~' && ch != '*' && ch != '/') {
    ungetc(ch, stdin);
    return readNumber();
  } 

  if (ch == '+') {
    exp1 = parseExpression();
    exp2 = parseExpression();
    if (exp2 == EOF) {
      exit(FAIL_INPUT);
    }
    checkAdd(exp1, exp2);
    return exp1 + exp2;
  } 
  
  if (ch == '~') {
    exp1 = parseExpression();
    exp2 = parseExpression();
    if (exp2 == EOF) {
      exit(FAIL_INPUT);
    }
    checkSub(exp1, exp2);
    return exp1 - exp2;
  } 
  
  if (ch == '*') {
    exp1 = parseExpression();
    exp2 = parseExpression();
    if (exp2 == EOF) {
      exit(FAIL_INPUT);
    }
    checkMul(exp1, exp2);
    return exp1 * exp2;
  } 
  
  if (ch == '/') {
    exp1 = parseExpression();
    exp2 = parseExpression();
    if (exp2 == EOF) {
      exit(FAIL_INPUT);
    }
    checkDiv(exp1, exp2);
    return exp1 / exp2;
  }
  exit(FAIL_INPUT);
}

/** 
  Program starting point, reads characters from standard input until end-of-file.
  @return program exit status
*/
int main()
{
  signed long ch = getchar();
  // Read all the characters from standard input.
  while (ch != EOF) {
    if (ch == ' ' || ch == '\n') {
      ch = getchar();
    } else {
      ungetc(ch, stdin);
      printNumber(parseExpression());
      printf("%c", NEW_LINE);
      ch = getchar();
    }
  }
  return EXIT_SUCCESS;
}
