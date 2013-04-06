#include "utility.h"

int String_to_Int(int string_type, char *string_digit)
{
	 int ret;

	if (string_type == OCT_STRING)
		sscanf(string_digit, "%o", &ret);
	else if (string_type == DEC_STRING)
		sscanf(string_digit, "%d", &ret);
	else if (string_type == HEX_STRING)
		sscanf(string_digit, "%x", &ret);
	else
		perror("Error: (Function)String_to_UnsignedInt\n");

	return ret;
}

float String_to_Float(char *string_digit)
{
	float ret;
	sscanf(string_digit, "%f", &ret);
	return ret;
}
