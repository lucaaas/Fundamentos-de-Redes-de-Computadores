#include <stdio.h>
#include <string.h>
#include "math_functions.h"

int add (int number1, int number2) {
  int result = number1 + number2;
  return result;
}

int subtract (int number1, int number2) {
  int result = number1 - number2;
  return result;
}

int multiply (int number1, int number2) {
  int result = number1 * number2;
  return result;
}

int divide (int number1, int number2) {
  if (number2 != 0) {
    int result = number1 / number2;
    return result;
  }
  else {
    return DIV_ERROR;
  }
}

int calculate (char* operator, int number1, int number2) {
  int result = -1;

  if (!strcmp (operator, "add")) {
    result = add (number1, number2);
  }

  else if (!strcmp (operator, "sub")) {
    result = subtract (number1, number2);
  }

  else if (!strcmp (operator, "mul")) {
    result = multiply (number1, number2);
  }

  else {
    result = divide (number1, number2);
  }

  return result;
}
