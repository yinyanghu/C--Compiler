#include "LexChecker.h"

void LexChecker(int lineno, char *text, char *error_type, char *message)
{
	if (message == NULL)
		printf("Error type A at line %d: %s \'%s\', %s\n", lineno, error_type, text, message);
	else
		printf("Error type A at line %d: %s \'%s\'\n", lineno, error_type, text);
}
