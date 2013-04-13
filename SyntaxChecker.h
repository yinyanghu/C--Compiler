#ifndef SYNTAXCHECKER_H
#define SYNTAXCHECKER_H

#include <stdio.h>

extern int ErrorCounter;

//Error type B
void SyntaxChecker(int lineno, char *message);
void SyntaxErrorReport(int lineno, char *message);

#endif
