#ifndef S21_SMARTCALC_H
#define S21_SMARTCALC_H
#include "s21_stack.h"

void ParserToStack(char *string, Node **base, double x_value, int *error);
double ConvertDouble(char *string, int *step, int *error);
void FillNode(Node **base, char symb, double x_value);
int IsDigit(char symb);
int chech_count_dot(char *string);
void ReverseStackAndSearchUnarOp(Node *base, Node **donor);
void ReversePolishNotation(Node **base, Node **end, int *error);
void CalculateMain(Node **base, double *answer, int *error);
double CalculateOneOper(double number, int type);
double CalculateTwoOper(double number_one, double number_two, int type);
void s21_toshortstring_function(char *fullstring, const char *find,
                                const char *replace);
void s21_toshortstring_full(char *fullstring);
int check_errors_two(double number_one, double number_two, int type);
int check_error_one(double number, int type);
double s21_smart_calc_func(char *string, double x_value, int *error);
int s21_str_validator(Node *base);
void validator_helper(Node *base, int *status, int *modul_bracket);
#endif
