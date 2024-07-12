#include "s21_stack.h"

int PopNode(Node **base) {
  Node *reNode = NULL;
  int status = CORRECT;
  if (*base != NULL) {
    reNode = (*base)->pred;
    free(*base);
    *base = reNode;
  } else {
    status = INCORRECT;
  }
  return status;
}

int PushNode(Node **base, double value, int type, int priority,
             char short_name) {
  int status = CORRECT;
  Node *newNode = (Node *)calloc(1, sizeof(Node));
  if (newNode) {
    newNode->value = value;
    newNode->type = type;
    newNode->priority = priority;
    newNode->pred = *base;
    newNode->short_name = short_name;
    *base = newNode;
  } else {
    status = INCORRECT;
  }

  return status;
}

void DestroyStack(Node **base) {
  while (PopNode(base) == CORRECT) {
  }
}

void print_Stack(Node *base) {
  while (base != NULL) {
    if (base->type == NUMBER) {
      printf("\nValue %lf, type %d, priority %d shortname %lf\n", base->value,
             base->type, base->priority, base->value);
    } else {
      printf("\nValue %lf, type %d, priority %d shortname %c\n", base->value,
             base->type, base->priority, base->short_name);
    }
    base = base->pred;
  }
  printf("\n");
}
