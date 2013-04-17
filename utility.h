#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

#define OCT_STRING	8
#define DEC_STRING	10
#define HEX_STRING	16


//#define PRINT_INDENT(Y)	printf("%"#Y"s","")

#define PRINT(WS,X)		print_indent(WS); \
						printf("%s",X)

#define PRINT_INT(X)	printf("%d",X)
#define PRINT_FLOAT(X)	printf("%.6f",X)
#define ENDL			printf("\n")

#define IPrint(X)		++indent; \
						PRINT(indent << 1,X); ENDL; \
						--indent;
#define LINE(V)			printf(" (%d)", TreeNode_GetLineno(&(V -> tree)));

void print_indent(int);

int String_to_Int(int, char *);

float String_to_Float(char *);

#endif
