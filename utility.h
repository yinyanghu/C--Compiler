#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

#define OCT_STRING	8
#define DEC_STRING	10
#define HEX_STRING	16



unsigned int String_to_UnsignedInt(int string_type, char *string_digit);

float String_to_Float(char *string_digit);

#endif
