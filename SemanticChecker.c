#include "SemanticChecker.h"
#include "SymbolsTable.h"
#include "const.h"
#include "AST.h"
#include <stdio.h>

void SemanticChecker(int lineno, int error_type, char *message)
{
	++ ErrorCounter;
	if (message != NULL)
		fprintf(stderr, "Error Type %d at line %d: Semantic Error, %s\n", error_type, lineno, message);
	else
		fprintf(stderr, "Error Type %d at line %d: Semantic Error\n", error_type, lineno);
}


void SemanticsAnalysis()
{
	SymbolsTableID_clear(STID);
	SymbolsTableNumber_clear(STNUM);
}

