#include "SyntaxChecker.h"
#include "const.h"

void SyntaxChecker(int lineno, char *message)
{
#ifdef GLOBAL_COLOR
	fprintf(stderr, RED);
#endif

	fprintf(stderr, "Syntax Error!\n");

#ifdef GLOBAL_COLOR
	fprintf(stderr, NORMAL);
#endif
}
