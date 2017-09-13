#include <stdio.h>
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
    int error = 2;
    return error;
  }
}
