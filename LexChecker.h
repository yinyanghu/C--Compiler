#ifndef LEXCHECKER_H
#define LEXCHECKER_H

#include "const.h"
#include <stdio.h>

int LexErr_Counter;

//Error type A
void LexChecker(int lineno, char *text, char *error_type, char *message);

#endif
