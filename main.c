#include <stdio.h>
#include <string.h>
#include "const.h"
#include "utility.h"
#include "LexChecker.h"
#include "SyntaxChecker.h"
#include "AST.h"
#include "syntax.tab.h"

struct Program		*AST;

int ErrorCounter;

int main(int argc, char **argv)
{
	/*yydebug = 1;*/

	if (argc != 2)
	{
		fprintf(stderr, "cmm: fatal error: No input files or Not only one input files\n");
		return -1;
	}

	if (strcmp(argv[1], "--version") == 0)
	{
		fprintf(stdout, "cmm: C Minus Minus Compiler 0.2\n");
		fprintf(stdout, "This is a free software written by Jian Li.\n");
		return 0;
	}

	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		fprintf(stderr, "cmm: fatal error: %s: No such file or directory\n", argv[1]); 
		return -1;
	}
	
	ErrorCounter = 0;

	yyrestart(f);
	yyparse();
	fclose(f);

	if (ErrorCounter == 0)
		Visit_Program(AST);

	return 0;	
}
