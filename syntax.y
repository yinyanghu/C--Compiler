%{
	#include <stdio.h>
	#include "lex.yy.c"
	#include "SyntaxChecker.h"
%}

%union
{
	int		key_int;
	float	key_float;
	char*	key_string;
}

%token		INT
%token		FLOAT
%token		ID
%token		SEMI
%token		COMMA
%token		ASSIGNOP
%token		RELOP
%token		PLUS
%token		MINUS
%token		STAR
%token		DIV
%token		AND
%token		OR
%token		DOT
%token		NOT
%token		TYPE
%token		LP
%token		RP
%token		LB
%token		RB
%token		LC
%token		RC
%token		STRUCT
%token		RETURN
%token		IF
%token		ELSE
%token		WHILE


%left		COMMA
%right		ASSIGNOP
%left		OR
%left		AND
%left		RELOP
%left		PLUS MINUS
%left		STAR DIV
%right		NOT
%left		LB RB LP RP DOT
%nonassoc	LOWER_THAN_ELSE
%nonassoc	ELSE


%%

Program				:	ExtDefList
						{
						}
					;

ExtDefList			:	ExtDef ExtDefList
						{
						}
					|	
						{
						}
					;

ExtDef				:	Specifier ExtDecList SEMI
						{
						}
					|	Specifier SEMI
						{
						}
					|	Specifier FunDec CompSt
						{
						}
					|	error SEMI
						{
						}
					;

ExtDecList			:	VarDec
						{
						}
					|	VarDec COMMA ExtDecList
						{
						}
					;





Specifier			:	TYPE
						{
						}
					|	StructSpecifier
						{
						}
					;

StructSpecifier		:	STRUCT OptTag LC DefList RC
						{
						}
					|	STRUCT Tag
						{
						}
					;

OptTag				:	ID
						{
						}
					|
						{
						}
					;

Tag					:	ID
						{
						}
					;







VarDec				:	ID
						{
						}
					|	VarDec LB INT RB
						{
						}
					;

FunDec				:	ID LP VarList RP
						{
						}
					|	ID LP RP
						{
						}
					;

VarList				:	ParamDec COMMA VarList
						{
						}
					|	ParamDec
						{
						}
					;

ParamDec			:	Specifier VarDec
						{
						}
					;




CompSt				:	LC DefList StmtList RC
						{
						}
					;

StmtList			:	Stmt StmtList
						{
						}
					|
						{
						}
					;

Stmt				:	Exp SEMI
						{
						}
					|	CompSt
						{
						}
					|	RETURN Exp SEMI
						{
						}
					|	IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
						{
						}
					|	IF LP Exp RP Stmt ELSE Stmt
						{
						}
					|	WHILE LP Exp RP Stmt
						{
						}
					|	error SEMI
						{
						}
					;

DefList				:	Def DefList
						{
						}
					|
						{
						}
					;

Def					:	Specifier DecList SEMI
						{
						}
					;

DecList				:	Dec
						{
						}
					|	Dec COMMA DecList
						{
						}
					;

Dec					:	VarDec
						{
						}
					|	VarDec ASSIGNOP Exp
						{
						}
					;




Exp					:	Exp ASSIGNOP Exp
						{
						}
					|	Exp AND Exp
						{
						}
					|	Exp OR Exp
						{
						}
					|	Exp RELOP Exp
						{
						}
					|	Exp PLUS Exp
						{
						}
					|	Exp MINUS Exp
						{
						}
					|	Exp STAR Exp
						{
						}
					|	Exp DIV Exp
						{
						}
					|	LP Exp RP
						{
						}
					|	MINUS Exp
						{
						}
					|	NOT Exp
						{
						}
					|	ID LP Args RP
						{
						}
					|	ID LP RP
						{
						}
					|	Exp LB Exp RB
						{
						}
					|	Exp DOT ID
						{
						}
					|	ID
						{
						}
					|	INT
						{
						}
					|	FLOAT
						{
						}
					|	LP error RP
						{
						}
					|	Exp LB error RB
						{
						}
					;

Args				:	Exp COMMA Args
						{
						}
					|	Exp
						{
						}
					;
%%

yyerror(char *msg)
{
	SyntaxChecker(0, "xxx");
	//fprintf(stderr, "error!\n");
}

