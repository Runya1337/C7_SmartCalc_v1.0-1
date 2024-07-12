#ifndef S21_STACK_H
#define S21_STACK_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

#define CORRECT 0
#define INCORRECT 1
#define NUMBER 0
#define SIN 1
#define COS 2
#define TAN 3
#define ACOS 4
#define ASIN 5
#define ATAN 6
#define SQRT 7
#define LN 8
#define LOG 9
#define MOD 10
#define BRACKET_OPEN 11
#define PLUS 12
#define MINUS 13
#define MUL 14
#define DIV 15
#define DEG 16
#define UN_PLUS 17
#define UN_MINUS 18
#define BRACKET_CLOSE 19
#define FUNCTIONS_COUNT 10

typedef struct Node {
  double value;
  int type;
  int priority;
  char short_name;
  struct Node *pred;
} Node;

int PopNode(Node **base);
int PushNode(Node **base, double value, int type, int priority,
             char short_name);
void print_Stack(Node *base);
void DestroyStack(Node **base);
#endif