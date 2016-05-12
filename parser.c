// parser.c
// Aidan Holloway-Bidwell
// Programming Languages, Spring 2016

#include "value.h"
#include "talloc.h"
#include "linkedlist.h"
#include "stdio.h"
#include "assert.h"

#ifndef _PARSER
#define _PARSER

Value *addToParseTree(Value *tree, int *depth, Value *token) {
  Value *current = tree;
  //Value *toPush = 0;
  if (token->type == OPEN_TYPE) {
    (*depth) ++;
    current = cons(token, current);
    //stack = cons(token, stack);
  } else if (token->type == CLOSE_TYPE) {
    (*depth) --;
    Value *subTree = makeNull();
    while ((cdr(current)->type != NULL_TYPE) && (car(current)->type != OPEN_TYPE)) {
      subTree = cons(car(current), subTree);
      current = cdr(current);
    }
    current = cdr(current);
    current = cons(subTree, current);
  } else {
    current = cons(token, current);
  }
  return current;
}

void syntaxError(int depth) {
  if (depth > 0) {
    printf("Syntax error: not enough close parentheses\n");
  } else {
    printf("Syntax error: too many close parentheses\n");
  }
  texit(1);
}

// Takes a list of tokens from a Racket program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens) {
  Value *tree = makeNull();
  Value *current = tokens;
  int depth = 0;

  assert(current != NULL && "Error (parse): null pointer");
  while (current->type != NULL_TYPE) {
    Value *token = car(current);
    tree = addToParseTree(tree, &depth, token);
    current = cdr(current);
  }
  if (depth != 0) {
    syntaxError(depth); // error case 4
  }
  tree = reverse(tree);
  return tree;
}

// Prints the tree to the screen in a readable fashion. It should look just like
// Racket code; use parentheses to indicate subtrees.
void printTree(Value *tree) {
  Value *curNode = tree;
  if (curNode->type == CONS_TYPE) {
    if (curNode->c.car->type == CONS_TYPE) {
      printf("(");
      printTree(curNode->c.car);
      printf(")");
    } else {
      printTree(curNode->c.car);
      }
    if (curNode->c.cdr->type != NULL_TYPE) {
      printf(" ");
    }
    printTree(curNode->c.cdr);
  } else if (curNode->type == INT_TYPE) {
    printf("%i", curNode->i);
  } else if (curNode->type == DOUBLE_TYPE) {
    printf("%f", curNode->d);
  } else if (curNode->type != NULL_TYPE) {
    printf("%s", curNode->s);
  }
}


#endif
