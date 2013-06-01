#include <stdio.h>
#include "cmm.h"

void LexChecker(int lineno, char *text, char *error_type, char *message)
{
	++ ErrorCounter;
	if (message != NULL)
		fprintf(stderr, "Error type A at line %d: %s \'%s\', %s\n", lineno, error_type, text, message);
	else
		fprintf(stderr, "Error type A at line %d: %s \'%s\'\n", lineno, error_type, text);
}
