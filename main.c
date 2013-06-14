#include <stdio.h>
#include <string.h>
#include "cmm.h"

struct Program		*AST;

struct IRChain		*IR;

int ErrorCounter;

void SyntaxAnalysis(FILE *file)
{
	yyrestart(file);
	yyparse();
	fclose(file);
}

int main(int argc, char **argv)
{
	/*yydebug = 1;*/

	if (argc != 3 && argc != 2)
	{
		fprintf(stderr, "cmm: fatal error: Input error\n");
		return -1;
	}

	if (strcmp(argv[1], "--version") == 0)
	{
		fprintf(stdout, "cmm: C Minus Minus Compiler 0.2\n");
		fprintf(stdout, "This is a free software written by Jian Li.\n");
		return 0;
	}

	FILE *f = fopen(argv[1], "r");

/*FILE *f = fopen("elixir.c", "r");*/

	if (!f)
	{
		fprintf(stderr, "cmm: fatal error: %s: No such file or directory\n", argv[1]); 
		return -1;
	}
	
	ErrorCounter = 0;

	SyntaxAnalysis(f);

	if (ErrorCounter != 0) return 0;

	/*Visit_Program(AST);*/
	SemanticAnalysis(AST);

	if (ErrorCounter != 0) return 0;

	FILE *ir_file = fopen(argv[2], "w");

	if (!ir_file)
	{
		fprintf(stderr, "cmm: fatal error: IR code file error\n"); 
		return -1;
	}

	GeneratingIR(AST, ir_file);

	fclose(ir_file);

	//IR_Optimizer(IR);

	return 0;
}
