#include "SyntaxChecker.h"
#include "const.h"

int LastErrorLineno = -1;

void SyntaxErrorReport(char *message)
{
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
