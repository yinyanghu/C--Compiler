%{
	#include "utility.h"

	int yycolumnno = 1;
%} 

upperletter		[A-Z]
lowerletter		[a-z]
underscore		[_]
letter			({upperletter}|{lowerletter}|{underscore})
digit			[0-9]

relop			">"|"<"|">="|"<="|"=="|"!="

hex				0[xX][0-9a-fA-F]+
oct				0[0-7]+
dec				0|([1-9]+[0-9]*)

float_A			{digit}+\.{digit}+
float_B_base	((\.{digit}+)|({digit}+\.{digit}+)|({digit}+\.))
float_B			{float_B_base}[eE][+-]?{digit}+
floating		{float_A}|{float_B}

hex_error		0[xX][{digit}|{letter}]*
oct_error		0[{digit}|{letter}]*
dec_error		[1-9][{digit}|{letter}]*
float_error		(\.{digit}+)|({digit}+\.)|(\.[Ee][+-]?{digit}+)|({float_B_base}[Ee][+-]?)

id				{letter}+[{letter}|{digit}]*

%option		yylineno

%%

";"				{
					printf("SEMI\n");
					//return SEMI;
				}

","				{
					printf("COMMA\n");
					//return COMMA;
				}

"="				{
					printf("ASSIGNOP\n");
					//return ASSIGNOP;
				}

{relop}			{
					printf("RELOP\n");
					//return RELOP;
				}

"+"				{
					printf("PLUS\n");
					//return PLUS;
				}

"-"				{
					printf("MINUS\n");
					//return MINUS;
				}

"*"				{
					printf("STAR\n");
					//return STAR;
				}

"/"				{
					printf("DIV\n");
					//return DIV;
				}

"&&"			{
					printf("AND\n");
					//return AND;
				}

"||"			{
					printf("OR\n");
					//return OR;
				}

"."				{
					printf("DOT\n");
					//return DOT;
				}

"!"				{
					printf("NOT\n");
					//return NOT;
				}

int				{
					printf("TYPE\n");
					//return TYPE;
				}

float			{
					printf("TYPE\n");
					//return TYPE;
				}

"("				{
					printf("LP\n");
					//return LP;
				}

")"				{
					printf("RP\n");
					//return RP;
				}

"["				{
					printf("LB\n");
					//return LB;
				}

"]"				{
					printf("RB\n");
					//return RB;
				}

"{"				{
					printf("LC\n");
					//return LC;
				}

"}"				{
					printf("RC\n");
					//return RC;
				}

struct			{
					printf("STRUCT\n");
					//return STRUCT;
				}

return			{
					printf("RETURN\n");
					//return RETURN;
				}

if				{
					printf("IF\n");
					//return IF;
				}

else			{
					printf("ELSE\n");
					//return ELSE;
				}

while			{
					printf("WHILE\n");
					//return WHILE;
				}

{hex}			{
					unsigned int key;
					printf("INT(HEX): ");
					key = String_to_UnsignedInt(HEX_STRING, yytext);
					printf("%d\n", key);
					//return INT;
				}

{oct}			{
					unsigned int key;
					printf("INT(OCT): ");
					key = String_to_UnsignedInt(OCT_STRING, yytext);
					printf("%d\n", key);
					//return INT;
				}

{dec}			{
					unsigned int key;
					printf("INT(DEC): ");
					key = String_to_UnsignedInt(DEC_STRING, yytext);
					printf("%d\n", key);
					//return INT;
				}

{hex_error}		{
					printf("Error type A at line %d: Illegal hexadecimal number \'%s\'\n", yylineno, yytext);
				}

{oct_error}		{
					printf("Error type A at line %d: Illegal octal number \'%s\'\n", yylineno, yytext);
				}


{dec_error}		{
					printf("Error type A at line %d: Illegal decimal number \'%s\'\n", yylineno, yytext);
				}

{float_error}	{
					printf("Error type A at line %d: Illegal floating point number \'%s\'\n", yylineno, yytext);
				}

{id}			{
					printf("ID: %s\n", yytext);
					//return ID;
				}

[ \t]+			{}

\n				{
					++ yycolumnno;
				}

.				{
					printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext);
				}

%%

int main(int argc, char **argv)
{
	yylex();
	printf("Success!\n");
	return 0;
}
