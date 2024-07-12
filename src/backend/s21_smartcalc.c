#include "s21_smartcalc.h"

void ParserToStack(char *string, Node **base, double x_value, int *error) {
  int step = 1;
  double value = 0;
  s21_toshortstring_full(string);
  while (*string != '\0' && *error != 1) {
    step = 1;
    if (IsDigit(*string) || *string == '.') {
      value = ConvertDouble(string, &step, error);
      PushNode(base, value, NUMBER, 1, *string);
    } else {
      FillNode(base, *string, x_value);
    }
    string += step;
  }
  if (DEBUG) {
    printf("\nSHORT STRING %s\n", string);
    printf("Спаршеная строка\n");
    print_Stack(*base);
  }
}

int IsDigit(char symb) {
  int status = 0;
  if (symb >= '0' && symb <= '9') {
    status = 1;
  }
  return status;
}

double ConvertDouble(char *string, int *step, int *error) {
  char buf[128] = {0};
  double value = 0;
  int counter_dot = 0;
  for (int i = 0; IsDigit(*string) || *string == '.'; i++) {
    if (*string == '.') {
      counter_dot += 1;
    }
    buf[i] = *string;
    string += 1;
  }
  if (counter_dot > 1) {
    *error = 1;
  }
  *step = strlen(buf);
  sscanf(buf, "%lf", &value);
  return value;
}

void FillNode(Node **base, char symb, double x_value) {
  int listPriority[20][4] = {
      {BRACKET_OPEN, '(', 0}, {BRACKET_CLOSE, ')', 0}, {SIN, 's', 4},
      {COS, 'c', 4},          {TAN, 't', 4},           {ACOS, 'z', 4},
      {ASIN, 'y', 4},         {ATAN, 'v', 4},          {SQRT, 'q', 4},
      {LN, 'h', 4},           {MOD, 'm', 2},           {LOG, 'l', 4},
      {DEG, '^', 4},          {PLUS, '+', 2},          {MINUS, '-', 2},
      {MUL, '*', 3},          {DIV, '/', 3},           {NUMBER, 'x', 1}};
  if (symb == 'x') {
    PushNode(base, x_value, NUMBER, 1, 'x');
  } else {
    for (int i = 0; i < 20; i++) {
      if (listPriority[i][1] == symb) {
        PushNode(base, -1, listPriority[i][0], listPriority[i][2],
                 listPriority[i][1]);
      }
    }
  }
}

void ReverseStackAndSearchUnarOp(Node *base, Node **donor) {
  int status = 0;
  while (base != NULL) {
    PushNode(donor, base->value, base->type, base->priority, base->short_name);
    base = base->pred;
    if (donor != NULL) {
      if (base != NULL) {
        if (base->type == BRACKET_OPEN || base->type == PLUS ||
            base->type == MINUS || base->type == COS) {
          status = 1;
        }
      } else if (base == NULL) {
        status = 1;
      }
    }
    if (status == 1) {
      if ((*donor)->type == MINUS) {
        (*donor)->type = UN_MINUS;

        (*donor)->priority = 3;
      } else if ((*donor)->type == PLUS) {
        (*donor)->type = UN_PLUS;
        (*donor)->priority = 3;
      }
    }
    status = 0;
  }
  if (DEBUG) {
    printf("Перевернутая  строка\n");
    print_Stack(*donor);
  }
}

void ReversePolishNotation(Node **base, Node **end, int *error) {
  Node *operators = NULL;
  while (*base != NULL && *error == 0) {
    if (((*base)->type) == NUMBER) {
      PushNode(end, (*base)->value, (*base)->type, (*base)->priority,
               (*base)->short_name);
    } else if (((*base)->type) == BRACKET_OPEN) {
      PushNode(&operators, (*base)->value, (*base)->type, (*base)->priority,
               (*base)->short_name);
    } else if (((*base)->type) == BRACKET_CLOSE) {
      while (operators != NULL && operators->type != BRACKET_OPEN) {
        PushNode(end, operators->value, operators->type, operators->priority,
                 operators->short_name);
        PopNode(&operators);
      }
      if (operators != NULL) {
        PopNode(&operators);
      } else {
        *error = 1;
      }
    } else {
      while (operators != NULL && ((*base)->priority) <= operators->priority &&
             (((*base)->type != UN_MINUS || operators->type != UN_MINUS) &&
              ((*base)->type != UN_PLUS || operators->type != UN_PLUS) &&
              ((*base)->type != UN_MINUS || operators->type != UN_PLUS) &&
              ((*base)->type != UN_PLUS || operators->type != UN_MINUS))) {
        PushNode(end, operators->value, operators->type, operators->priority,
                 operators->short_name);
        PopNode(&operators);
      }
      PushNode(&operators, (*base)->value, (*base)->type, (*base)->priority,
               (*base)->short_name);
    }
    PopNode(base);
  }
  while (operators != NULL) {
    PushNode(end, operators->value, operators->type, operators->priority,
             operators->short_name);
    PopNode(&operators);
  }
  if (DEBUG) {
    printf("Польская строка\n");
    print_Stack(*end);
  }
  DestroyStack(&operators);
}

double CalculateTwoOper(double number_one, double number_two, int type) {
  double answer = 0;
  if (type == PLUS) {
    printf("%lf %lf\n", number_one, number_two);
    answer = number_one + number_two;
    printf("%lf\n", answer);
  } else if (type == MINUS) {
    answer = number_two - number_one;
  } else if (type == MUL) {
    answer = number_one * number_two;
  } else if (type == DIV) {
    answer = number_two / number_one;
  } else if (type == MOD) {
    answer = fmod(number_two, number_one);
  } else if (type == DEG) {
    answer = pow(number_two, number_one);
  }
  return answer;
}

int check_errors_two(double number_one, double number_two, int type) {
  if (DEBUG) {
    printf("\nНа вход пришло первое число %lf и второе число %lf с типом %d\n",
           number_one, number_two, type);
  }
  int status = CORRECT;
  if ((type == DIV && number_one == 0.0) ||
      (type == MOD && number_one == 0.0) ||
      (type == DEG && (fabs((number_one)) > abs((int)number_one)) &&
       number_two <= 0)) {
    status = INCORRECT;
  }
  return status;
}

int check_error_one(double number, int type) {
  if (DEBUG) {
    printf("\nНа вход пришло первое число %lf с типом %d\n", number, type);
  }
  int status = CORRECT;
  if (((type == ACOS || type == ASIN) && fabs(number) > 1) ||
      ((type == LOG || type == LN || type == SQRT) && (number < 0.0))) {
    status = INCORRECT;
  }
  return status;
}

double CalculateOneOper(double number, int type) {
  double answer = 0;
  if (type == SIN) {
    answer = sin(number);
  } else if (type == COS) {
    answer = cos(number);
  } else if (type == TAN) {
    answer = tan(number);
  } else if (type == ACOS) {
    answer = acos(number);
  } else if (type == ASIN) {
    answer = asin(number);
  } else if (type == ATAN) {
    answer = atan(number);
  } else if (type == SQRT) {
    answer = sqrt(number);
  } else if (type == LN) {
    answer = log(number);
  } else if (type == LOG) {
    answer = log10(number);
  } else if (type == UN_MINUS) {
    answer = -number;
  } else if (type == UN_PLUS) {
    answer = number;
  }
  return answer;
}

void CalculateMain(Node **base, double *answer, int *error) {
  Node *operandos = NULL;
  double number_1 = 0;
  double number_2 = 0;
  double number_3 = 0;
  while (*base != NULL && *error == CORRECT) {
    if ((*base)->type == NUMBER) {
      PushNode(&operandos, (*base)->value, (*base)->type, (*base)->priority,
               (*base)->short_name);
    } else if ((*base)->priority == 2 ||
               ((*base)->priority == 3 && ((*base)->type != UN_MINUS) &&
                ((*base)->type != UN_PLUS)) ||
               (*base)->type == DEG) {
      number_1 = operandos->value;
      PopNode(&operandos);
      number_2 = operandos->value;
      PopNode(&operandos);
      *error = check_errors_two(number_1, number_2, (*base)->type);
      if (*error == 0) {
        PushNode(&operandos,
                 CalculateTwoOper(number_1, number_2, (*base)->type), NUMBER, 1,
                 '0');
      }
    } else if ((*base)->priority == 4 || (*base)->type == UN_MINUS ||
               (*base)->type == UN_PLUS) {
      number_3 = operandos->value;
      PopNode(&operandos);
      *error = check_error_one(number_3, (*base)->type);
      if (*error == 0) {
        PushNode(&operandos, CalculateOneOper(number_3, (*base)->type), NUMBER,
                 1, '0');
      }
    }
    PopNode(base);
  }
  if (*error == 0) {
    *answer = operandos->value;
  }
  DestroyStack(&operandos);
}

void s21_toshortstring_full(char *fullstring) {
  char function_full_name_list[FUNCTIONS_COUNT][16] = {
      "asin", "acos", "atan", "cos", "sin", "tan", "sqrt", "ln", "mod", "log"};
  char function_short_name_list[FUNCTIONS_COUNT][16] = {
      "y", "z", "v", "c", "s", "t", "q", "h", "m", "l"};
  for (int i = 0; i < FUNCTIONS_COUNT; i++) {
    s21_toshortstring_function(fullstring, function_full_name_list[i],
                               function_short_name_list[i]);
  }
}

void s21_toshortstring_function(char *fullstring, const char *find,
                                const char *replace) {
  char *pos;
  int findLen = strlen(find);
  int replaceLen = strlen(replace);
  pos = strstr(fullstring, find);
  while (pos != NULL) {
    memmove(pos + replaceLen, pos + findLen, strlen(pos + findLen) + 1);
    memcpy(pos, replace, replaceLen);
    pos = strstr(pos + replaceLen, find);
  }
}

void validator_helper(Node *base, int *status, int *modul_bracket) {
  if ((base)->type == UN_PLUS || (base)->type == UN_MINUS) {
    if ((base)->pred->type == BRACKET_CLOSE) {
      *status = 1;
    }
  }
  if ((base)->type == BRACKET_OPEN) {
    *modul_bracket += 1;
    if (((base)->pred->type != UN_PLUS && (base)->pred->type != UN_MINUS) &&
        ((base)->pred->priority == 2 || (base)->pred->priority == 3 ||
         (base)->pred->type == BRACKET_CLOSE || (base)->pred->type == DEG)) {
      *status = 1;
    }
  }
  if ((base)->type == BRACKET_CLOSE) {
    *modul_bracket -= 1;
    if (((base)->pred->priority != 2 && (base)->pred->priority != 3) &&
        (base)->pred->type != DEG && (base)->pred->type == NUMBER) {
      *status = 1;
    }
  }
  if ((base)->priority == 4 && (base)->type != DEG) {
    if ((base)->pred->type != BRACKET_OPEN) {
      *status = 1;
    }
  }
  if ((base)->priority == 2 || (base)->priority == 3) {
    if ((base)->pred->priority == 2 ||
        ((base)->pred->priority == 3 && (base)->pred->type != UN_PLUS &&
         (base)->pred->type != UN_MINUS) ||
        (base)->pred->type == DEG) {
      *status = 1;
    }
  }
  if ((base)->type == DEG) {
    if ((base)->pred->priority != 4 && (base)->pred->priority != 1 &&
        (base)->pred->type != BRACKET_OPEN) {
      *status = 1;
    }
  }
  if ((base)->type == NUMBER) {
    if ((base)->pred->priority != 2 && (base)->pred->priority != 3 &&
        (base)->pred->type != DEG && (base)->pred->type != BRACKET_CLOSE) {
      *status = 1;
    }
  }
}

int s21_str_validator(Node *base) {
  int status = 0;
  int modul_bracket = 0;
  if (base->priority == 2 ||
      (base->priority == 3 && base->type != UN_MINUS &&
       base->type != UN_PLUS) ||
      base->type == DEG) {
    status = 1;
  }
  while (base->pred != NULL && !status) {
    validator_helper(base, &status, &modul_bracket);
    base = base->pred;
  }
  if (base->type == BRACKET_CLOSE) {
    modul_bracket -= 1;
  } else if (base->type != NUMBER) {
    status = 1;
  }
  if (modul_bracket != 0) {
    status = 1;
  }
  return status;
}

double s21_smart_calc_func(char *string, double x_value, int *error) {
  Node *base = NULL;
  Node *base_reverse = NULL;
  Node *pre_final = NULL;
  Node *final = NULL;
  int status_valid;
  double answer = 0;
  if (strlen(string) == 0 || strcmp(string, "BRUH") == 0) {
    *error = 1;
  } else {
    ParserToStack(string, &base, x_value, error);
    if (*error == 0) {
      ReverseStackAndSearchUnarOp(base, &base_reverse);
      DestroyStack(&base);
      status_valid = s21_str_validator(base_reverse);
      if (status_valid == 0) {
        ReversePolishNotation(&base_reverse, &pre_final, error);
        if (*error == 0) {
          ReverseStackAndSearchUnarOp(pre_final, &final);
          CalculateMain(&final, &answer, error);
        }
      } else {
        *error = 1;
      }
    }
  }

  DestroyStack(&base);
  DestroyStack(&pre_final);
  DestroyStack(&base_reverse);
  DestroyStack(&final);
  return answer;
}