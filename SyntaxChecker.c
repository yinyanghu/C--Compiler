#include "SyntaxChecker.h"
#include "const.h"
#include <stdio.h>

int LastErrorLineno = -1;

int LastErrorReport = -1;

void SyntaxErrorReport(int lineno, char *message)
{
	if (lineno == LastErrorReport) return;
	LastErrorReport = lineno;
	fprintf(stderr, "\t%s\n", message);
}

void SyntaxChecker(int lineno, char *message)
{
	++ ErrorCounter;
	if (lineno == LastErrorLineno) return;
	LastErrorLineno = lineno;

#ifdef GLOBAL_COLOR
	fprintf(stderr, RED);
#endif

	if (message == NULL)
		fprintf(stderr, "Error type B at line %d: Syntax Error\n", lineno);
	else
		fprintf(stderr, "Error type B at line %d: Syntax Error, %s\n", lineno, message);

#ifdef GLOBAL_COLOR
	fprintf(stderr, NORMAL);
#endif
}
