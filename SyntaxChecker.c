#include "SyntaxChecker.h"
#include "const.h"

void SyntaxChecker(int lineno, char *message)
{
#ifdef GLOBAL_COLOR
	fprintf(stderr, RED);
#endif

	++ ErrorCounter;
	if (message == NULL)
		fprintf(stderr, "Error type B at line %d: Syntax Error\n", lineno);
	else
		fprintf(stderr, "Error type B at line %d: Syntax Error, %s\n", lineno, message);

#ifdef GLOBAL_COLOR
	fprintf(stderr, NORMAL);
#endif
}
