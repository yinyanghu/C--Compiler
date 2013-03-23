#ifndef UTILITY_H
#define UTILITY_H

#define OCT_STRING	8
#define DEC_STRING	10
#define HEX_STRING	16


unsigned int String_to_UnsignedInt(int string_type, char *string_digit)
{
	unsigned int ret;

	if (string_type == OCT_STRING)
		sscanf(string_digit, "%o", &ret);
	else if (string_type == DEC_STRING)
		sscanf(string_digit, "%u", &ret);
	else if (string_type == HEX_STRING)
		sscanf(string_digit, "%x", &ret);
	else
		perror("Error: (Function)String_to_UnsignedInt\n");

	return ret;
}

#endif
