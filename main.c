#include <stdio.h>
#include <string.h>
#include "const.h"
#include "utility.h"
#include "LexChecker.h"
#include "SyntaxChecker.h"
#include "AST.h"
#include "syntax.tab.h"

struct Program		*AST;

int main(int argc, char **argv)
{
	//yydebug = 1;
	yyparse();

	printf("==========================================\n");

	Visit_Program(AST);

	return 0;	
}
