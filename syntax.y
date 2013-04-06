%{
	#include <stdio.h>
	#include "lex.yy.c"
	#include "AST.h"

	extern struct Program	*AST;
	void yyerror(char *msg);
%}

%union
{
	int						key_int;
	float					key_float;
	char					*key_string;

	BinaryOP_Relop			relop;
	DataType				datatype;

	struct Program			*prorgram;
	struct ExtDefList		*extdeflist;
	struct ExtDef			*extdef;
	struct ExtDecList		*extdeclist;
	struct Specifier		*specifier;
	struct StructSpecifier	*structspecifier;
	struct OptTag			*opttag;
	struct Tag				*tag;
	struct VarDec			*vardec;
	struct FunDec			*fundec;
	struct VarList			*varlist;
	struct ParamDec			*paramdec;
	struct CompSt			*compst;
	struct StmtList			*stmtlist;
	struct Stmt				*stmt;
	struct DefList			*deflist;
	struct Def				*def;
	struct DecList			*declist;
	struct Dec				*dec;
	struct Exp				*exp;
	struct Args				*args;
}

%token		<key_int> INT
%token		<key_float> FLOAT
%token		<key_string> ID
%token		SEMI
%token		COMMA
%token		ASSIGNOP
%token		<relop> RELOP
%token		PLUS
%token		MINUS
%token		STAR
%token		DIV
%token		AND
%token		OR
%token		DOT
%token		NOT
%token		<datatype> TYPE
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

%type		<program> Program
%type		<extdeflist> ExtDefList
%type		<extdef> ExtDef
%type		<extdeclist> ExtDecList
%type		<specifier> Specifier
%type		<structspecifier> StructSpecifier
%type		<opttag> OptTag
%type		<tag> Tag
%type		<vardec> VarDec
%type		<fundec> FunDec
%type		<varlist> VarList
%type		<paramdec> ParamDec
%type		<compst> CompSt
%type		<stmtlist> StmtList
%type		<stmt> Stmt
%type		<deflist> DefList
%type		<def> Def
%type		<declist> DecList
%type		<dec> Dec
%type		<exp> Exp
%type		<args> Args

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

%start		Program


%%

Program				:	ExtDefList
						{
							AST = Build_Program($1, 0);
						}
					;

ExtDefList			:	ExtDef ExtDefList
						{
							$$ = Build_ExtDefList($1, $2, 0);
						}
					|	
						{
							$$ = NULL;
						}
					;

ExtDef				:	Specifier ExtDecList SEMI
						{
							$$ = Build_ExtDef((void *)Build_ExtDef_A($1, $2), &Visit_ExtDef_A, 0);
						}
					|	Specifier SEMI
						{
							$$ = Build_ExtDef((void *)Build_ExtDef_B($1), &Visit_ExtDef_B, 0);
						}
					|	Specifier FunDec CompSt
						{
							$$ = Build_ExtDef((void *)Build_ExtDef_C($1, $2, $3), &Visit_ExtDef_C, 0);
						}
					|	error SEMI
						{
						}
					;

ExtDecList			:	VarDec
						{
							$$ = Build_ExtDecList($1, NULL, 0);
						}
					|	VarDec COMMA ExtDecList
						{
							$$ = Build_ExtDecList($1, $3, 0);
						}
					;





Specifier			:	TYPE
						{
							$$ = Build_Specifier((void *)Build_Specifier_A($1), &Visit_Specifier_A, 0);	
						}
					|	StructSpecifier
						{
							$$ = Build_Specifier((void *)Build_Specifier_B($1), &Visit_Specifier_B, 0);	
						}
					;

StructSpecifier		:	STRUCT OptTag LC DefList RC
						{
							$$ = Build_StructSpecifier((void *)Build_StructSpecifier_A($2, $4), &Visit_StructSpecifier_A, 0);
						}
					|	STRUCT Tag
						{
							$$ = Build_StructSpecifier((void *)Build_StructSpecifier_B($2), &Visit_StructSpecifier_B, 0);
						}
					;

OptTag				:	ID
						{
							$$ = Build_OptTag(Build_ID($1), 0);
						}
					|
						{
							$$ = NULL;
						}
					;

Tag					:	ID
						{
							$$ = Build_Tag(Build_ID($1), 0);
						}
					;







VarDec				:	ID
						{
							$$ = Build_VarDec((void *)Build_VarDec_A(Build_ID($1)), &Visit_VarDec_A, 0);
						}
					|	VarDec LB INT RB
						{
							$$ = Build_VarDec((void *)Build_VarDec_B($1, Build_TYPE_INT($3)), &Visit_VarDec_B, 0);
						}
					;

FunDec				:	ID LP VarList RP
						{
							$$ = Build_FunDec(Build_ID($1), $3, 0);
						}
					|	ID LP RP
						{
							$$ = Build_FunDec(Build_ID($1), NULL, 0);
						}
					;

VarList				:	ParamDec COMMA VarList
						{
							$$ = Build_VarList($1, $3, 0);
						}
					|	ParamDec
						{
							$$ = Build_VarList($1, NULL, 0);
						}
					;

ParamDec			:	Specifier VarDec
						{
							$$ = Build_ParamDec($1, $2, 0);
						}
					;




CompSt				:	LC DefList StmtList RC
						{
							$$ = Build_CompSt($2, $3, 0);
						}
					;

StmtList			:	Stmt StmtList
						{
							$$ = Build_StmtList($1, $2, 0);
						}
					|
						{
							$$ = NULL;
						}
					;

Stmt				:	Exp SEMI
						{
							$$ = Build_Stmt((void *)Build_Stmt_Exp($1), &Visit_Stmt_Exp, 0);
						}
					|	CompSt
						{
							$$ = Build_Stmt((void *)Build_Stmt_CompSt($1), &Visit_Stmt_CompSt, 0);
						}
					|	RETURN Exp SEMI
						{
							$$ = Build_Stmt((void *)Build_Stmt_Return($2), &Visit_Stmt_Return, 0);
						}
					|	IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
						{
							$$ = Build_Stmt((void *)Build_Stmt_If($3, $5), &Visit_Stmt_If, 0);
						}
					|	IF LP Exp RP Stmt ELSE Stmt
						{
							$$ = Build_Stmt((void *)Build_Stmt_If_Else($3, $5, $7), &Visit_Stmt_If_Else, 0);
						}
					|	WHILE LP Exp RP Stmt
						{
							$$ = Build_Stmt((void *)Build_Stmt_While($3, $5), &Visit_Stmt_While, 0);
						}
					|	error SEMI
						{
						}
					;

DefList				:	Def DefList
						{
							$$ = Build_DefList($1, $2, 0);
						}
					|
						{
							$$ = NULL;
						}
					;

Def					:	Specifier DecList SEMI
						{
							$$ = Build_Def($1, $2, 0);
						}
					;

DecList				:	Dec
						{
							$$ = Build_DecList($1, NULL, 0);
						}
					|	Dec COMMA DecList
						{
							$$ = Build_DecList($1, $3, 0);
						}
					;

Dec					:	VarDec
						{
							$$ = Build_Dec($1, NULL, 0);
						}
					|	VarDec ASSIGNOP Exp
						{
							$$ = Build_Dec($1, $3, 0);
						}
					;




Exp					:	Exp ASSIGNOP Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Assign($1, $3), &Visit_Exp_Assign, 0);
						}
					|	Exp AND Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Binary_Cal($1, $3, OP_AND), &Visit_Exp_Binary_Cal, 0);
						}
					|	Exp OR Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Binary_Cal($1, $3, OP_OR), &Visit_Exp_Binary_Cal, 0);
						}
					|	Exp RELOP Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Binary_Rel($1, $3, $2), &Visit_Exp_Binary_Rel, 0);
						}
					|	Exp PLUS Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Binary_Cal($1, $3, OP_PLUS), &Visit_Exp_Binary_Cal, 0);
						}
					|	Exp MINUS Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Binary_Cal($1, $3, OP_MINUS), &Visit_Exp_Binary_Cal, 0);
						}
					|	Exp STAR Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Binary_Cal($1, $3, OP_STAR), &Visit_Exp_Binary_Cal, 0);
						}
					|	Exp DIV Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Binary_Cal($1, $3, OP_DIV), &Visit_Exp_Binary_Cal, 0);
						}
					|	LP Exp RP
						{
							$$ = Build_Exp((void *)Build_Exp_Unary($2, OP_PAR), &Visit_Exp_Unary, 0);
						}
					|	MINUS Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Unary($2, OP_NEG), &Visit_Exp_Unary, 0);
						}
					|	NOT Exp
						{
							$$ = Build_Exp((void *)Build_Exp_Unary($2, OP_NOT), &Visit_Exp_Unary, 0);
						}
					|	ID LP Args RP
						{
							$$ = Build_Exp((void *)Build_Exp_Function(Build_ID($1), $3), &Visit_Exp_Function, 0);
						}
					|	ID LP RP
						{
							printf("Panic!\n");
							$$ = Build_Exp((void *)Build_Exp_Function(Build_ID($1), NULL), &Visit_Exp_Function, 0);
						}
					|	Exp LB Exp RB
						{
							$$ = Build_Exp((void *)Build_Exp_Array($1, $3), &Visit_Exp_Array, 0);
						}
					|	Exp DOT ID
						{
							$$ = Build_Exp((void *)Build_Exp_Attribute($1, Build_ID($3)), &Visit_Exp_Attribute, 0);
						}
					|	ID
						{
							$$ = Build_Exp((void *)Build_ID($1), &Visit_ID, 0);
						}
					|	INT
						{
							$$ = Build_Exp((void *)Build_TYPE_INT($1), &Visit_TYPE_INT, 0);
						}
					|	FLOAT
						{
							$$ = Build_Exp((void *)Build_TYPE_FLOAT($1), &Visit_TYPE_FLOAT, 0);
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
							$$ = Build_Args($1, $3, 0);
						}
					|	Exp
						{
							$$ = Build_Args($1, NULL, 0);
						}
					;
%%

void yyerror(char *msg)
{
	SyntaxChecker(0, "xxx");
	//fprintf(stderr, "error!\n");
}

