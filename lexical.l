%{

	#include "const.h"
	#include "utility.h"
	#include "LexChecker.h"
	#include "syntax.tab.h"
	#include <string.h>

	int yycolumnno = 1;

	#define YY_USER_ACTION	yylloc.first_line = yylloc.last_line = yylineno; \
							yylloc.first_column = yycolumnno; \
							yylloc.last_column = yycolumnno + yyleng - 1; \
							yycolumnno += yyleng;
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

hex_error		0[xX]({digit}|{letter})*
oct_error		0({digit}|{letter})*
dec_error		[1-9]({digit}|{letter})*
float_error		(\.{digit}+)|({digit}+\.)|(\.[Ee][+-]?{digit}+)|({float_B_base}[Ee][+-]?)|({digit}+[Ee][+-]?{digit}+)

id				{letter}+({letter}|{digit})*

%option		yylineno

%x			C_COMMENT

%%


<C_COMMENT><<EOF>>		{
							//printf("Error EOF!\n");
							LexChecker(yylineno, "character: EOF", "Incompleted Comment", "Exit!");
							exit(-1);
						}
<C_COMMENT>\n			{
							yycolumnno = 1;
						}
<C_COMMENT>.			{ }

<C_COMMENT>"/*"			{
							//printf("Error: /* is nested!\n");
							LexChecker(yylineno, yytext, "/* is nested!", "Ignore!");
						}

<C_COMMENT>"*/"			{ BEGIN(INITIAL); }


"//".*\n		{}

"/*"			{ BEGIN(C_COMMENT); }

";"				{
					//printf("SEMI\n");
					return SEMI;
				}

","				{
					//printf("COMMA\n");
					return COMMA;
				}

"="				{
					//printf("ASSIGNOP\n");
					return ASSIGNOP;
				}

{relop}			{
					//printf("RELOP\n");
					
					/* to fix the bug */
					return RELOP;
				}

"+"				{
					//printf("PLUS\n");
					return PLUS;
				}

"-"				{
					//printf("MINUS\n");
					return MINUS;
				}

"*"				{
					//printf("STAR\n");
					return STAR;
				}

"/"				{
					//printf("DIV\n");
					return DIV;
				}

"&&"			{
					//printf("AND\n");
					return AND;
				}

"||"			{
					//printf("OR\n");
					return OR;
				}

"."				{
					//printf("DOT\n");
					return DOT;
				}

"!"				{
					//printf("NOT\n");
					return NOT;
				}

int				{
					//printf("TYPE\n");
					yylval.datatype = TYPE_INT;
					return TYPE;
				}

float			{
					//printf("TYPE\n");
					yylval.datatype = TYPE_FLOAT;
					return TYPE;
				}

"("				{
					//printf("LP\n");
					return LP;
				}

")"				{
					//printf("RP\n");
					return RP;
				}

"["				{
					//printf("LB\n");
					return LB;
				}

"]"				{
					//printf("RB\n");
					return RB;
				}

"{"				{
					//printf("LC\n");
					return LC;
				}

"}"				{
					//printf("RC\n");
					return RC;
				}

struct			{
					//printf("STRUCT\n");
					return STRUCT;
				}

return			{
					//printf("RETURN\n");
					return RETURN;
				}

if				{
					//printf("IF\n");
					return IF;
				}

else			{
					//printf("ELSE\n");
					return ELSE;
				}

while			{
					//printf("WHILE\n");
					return WHILE;
				}

{hex}			{
					//unsigned int key;
					//printf("INT(HEX)\n");
					yylval.key_int = String_to_Int(HEX_STRING, yytext);
					//printf("%d\n", key);
					return INT;
				}

{oct}			{
					//unsigned int key;
					//printf("INT(OCT)\n");
					yylval.key_int = String_to_Int(OCT_STRING, yytext);
					//printf("%d\n", key);
					return INT;
				}

{dec}			{
					//unsigned int key;
					//printf("INT(DEC)\n");
					yylval.key_int = String_to_Int(DEC_STRING, yytext);
					//printf("%d\n", key);
					return INT;
				}

{floating}		{
					//float key;
					//printf("FLOAT\n");
					yylval.key_float = String_to_Float(yytext);
					//printf("%.6f\n", key);
					return FLOAT;
				}

{hex_error}		{
					//printf("Error type A at line %d: Illegal hexadecimal number \'%s\'\n", yylineno, yytext);
					LexChecker(yylineno, yytext, "Illegal hexadecimal number", "(Default set to 0)Continue!");

					//try to recovery
					yylval.key_int = 0;
					return INT;
				}

{oct_error}		{
					//printf("Error type A at line %d: Illegal octal number \'%s\'\n", yylineno, yytext);
					LexChecker(yylineno, yytext, "Illegal octal number", "(Default set to 0)Continue!");

					//try to recovery
					yylval.key_int = 0;
					return INT;
				}

{float_error}	{
					//printf("Error type A at line %d: Illegal floating point number \'%s\'\n", yylineno, yytext);
					LexChecker(yylineno, yytext, "Illegal floating point number", "(Default set to 0)Continue!");

					//try to recovery
					yylval.key_float = 0.0;
					return FLOAT;
				}

{dec_error}		{
					//printf("Error type A at line %d: Illegal decimal number \'%s\'\n", yylineno, yytext);
					LexChecker(yylineno, yytext, "Illegal decimal number", "(Default set to 0)Continue!");

					//try to recovery
					yylval.key_int = 0;
					return INT;
				}


{id}			{
					//printf("ID: %s\n", yytext);
					yylval.key_string = strdup(yytext);
					return ID;
				}

[ \t]+			{}

\n				{
					++ yycolumnno;
				}

.				{
					//printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext);
					LexChecker(yylineno, yytext, "Mysterious character", "Ignore!");
				}

%%

/*
int main(int argc, char **argv)
{
	yylex();
	printf("Successful Termination!\n");
	return 0;
}
*/
