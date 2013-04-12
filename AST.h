#ifndef AST_H
#define AST_H

//#include "list.h"
#include "const.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>


int indent;

struct TreeNode
{
	int lineno;
};

int TreeNode_GetLineno(struct TreeNode *ptr);

void TreeNode_SetLineno(struct TreeNode *ptr, int key);





struct Program
{
	struct TreeNode		tree;

	struct ExtDefList	*extdeflist;
};



struct ExtDefList
{
	struct TreeNode		tree;

	struct ExtDef		*extdef;
	struct ExtDefList	*extdeflist;
};



struct ExtDef
{
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);
	/*
	union
	{
		struct ExtDef_A		*A;
		struct ExtDef_B		*B;
		struct ExtDef_C		*C;
	};
	*/
};



struct ExtDef_A
{
	struct Specifier	*specifier;
	struct ExtDecList	*extdeflist;
};

struct ExtDef_B
{
	struct Specifier	*specifier;
};


struct ExtDef_C
{
	struct Specifier	*specifier;
	struct FunDec		*fundec;
	struct CompSt		*compst;
};





struct ExtDecList
{
	struct TreeNode		tree;

	struct VarDec		*vardec;
	struct ExtDecList	*extdeclist;

};






struct Specifier
{
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);
	/*
	union
	{
		struct Specifier_A	*A;
		struct Specifier_B	*B;
	};
	*/
};


struct Specifier_A
{
	DataType	type;
};

struct Specifier_B
{
	struct StructSpecifier	*structspecifier;
};





struct StructSpecifier
{
	struct TreeNode		tree;

	void		*next;

	void	(*Visit)(void *);
	/*
	union
	{
		struct StructSpecifier_A	*A;
		struct StructSpecifier_B	*B;
	};
	*/
};

struct StructSpecifier_A
{
	struct OptTag		*opttag;
	struct DefList		*deflist;
};

struct StructSpecifier_B
{
	struct Tag		*tag;
};




struct OptTag
{
	struct TreeNode		tree;

	struct ID			*id;
};




struct Tag
{
	struct TreeNode		tree;

	struct ID			*id;
};






struct VarDec
{
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);

	/*
	union
	{
		struct VarDec_A		*A;
		struct VarDec_B		*B;
	};
	*/
};




struct VarDec_A
{
	struct ID		*id;
};


struct VarDec_B
{
	struct VarDec		*vardec;
	struct TYPE_INT		*type_int;
};






struct FunDec
{
	struct TreeNode		tree;

	struct ID			*id;
	struct VarList		*varlist;
};



struct VarList
{
	struct TreeNode		tree;

	struct ParamDec		*paramdec;
	struct VarList		*varlist;
};





struct ParamDec
{
	struct TreeNode		tree;

	struct Specifier	*specifier;
	struct VarDec		*vardec;
};





struct CompSt
{
	struct TreeNode		tree;

	struct DefList		*deflist;
	struct StmtList		*stmtlist;
};





struct StmtList
{
	struct TreeNode		tree;

	struct Stmt			*stmt;
	struct StmtList		*stmtlist;
};


struct Stmt
{
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);
	/*
	union
	{
		struct Stmt_Exp		*stmt_exp;
		struct Stmt_CompSt	*stmt_compst;
		struct Stmt_Return	*stmt_return;
		struct Stmt_If		*stmt_if;
		struct Stmt_If_Else	*stmt_if_else;
		struct Stmt_While	*stmt_while;
	};
	*/
};

struct Stmt_Exp
{
	struct Exp		*exp;
};

struct Stmt_CompSt
{
	struct CompSt	*compst;
};

struct Stmt_Return
{
	struct Exp		*exp;
};

struct Stmt_If
{
	struct Exp		*exp;
	struct Stmt		*stmt;
};


struct Stmt_If_Else
{
	struct Exp		*exp;
	struct Stmt		*stmt_if, *stmt_else;
};


struct Stmt_While
{
	struct Exp		*exp;
	struct Stmt		*stmt;
};








struct DefList
{
	struct TreeNode		tree;

	struct Def			*def;
	struct DefList		*deflist;
};

struct Def
{
	struct TreeNode		tree;

	struct Specifier	*specifier;
	struct DecList		*declist;
};

struct DecList
{
	struct TreeNode		tree;
	
	struct Dec			*dec;
	struct DecList		*declist;
};

struct Dec
{
	struct TreeNode		tree;

	struct VarDec		*vardec;
	struct Exp			*exp;
};

struct Exp
{
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);
	/*
	union
	{
		struct Exp_Assign				*exp_ass;
		struct Exp_Binary_Rel			*exp_rel;
		struct Exp_Binary_Cal			*exp_cal;
		struct Exp_Unary				*exp_unary;
		struct Exp_Function				*exp_func;
		struct Exp_Array				*exp_array;
		struct Exp_Attribute			*exp_attr;
		struct Exp_Variable				*exp_var;
		struct Exp_Int					*exp_int;
		struct Exp_Float				*exp_float;
	};
	*/
};


struct Exp_Assign
{
	struct Exp		*exp_left, *exp_right;
};

struct Exp_Binary_Rel
{
	struct Exp		*exp_a, *exp_b;
	BinaryOP_Relop	op;
};

struct Exp_Binary_Cal
{
	struct Exp		*exp_a, *exp_b;
	BinaryOP_Calop	op;
};

struct Exp_Unary
{
	struct Exp		*exp;
	UnaryOP			op;
};

struct Exp_Function
{
	struct ID		*func;
	struct Args		*args;
};

struct Exp_Array
{
	struct Exp		*array, *index;
};

struct Exp_Attribute
{
	struct Exp		*exp;
	struct ID		*attribute;

};

struct Exp_Variable
{
	struct ID		*var;
};

struct TYPE_INT
{
	int		key;
};

struct TYPE_FLOAT
{
	float	key;
};

struct Args
{
	struct TreeNode		tree;

	struct Exp		*exp;
	struct Args		*args;
};

struct ID
{
	char	name[STRING_SIZE];
};






struct Program				*Build_Program(struct ExtDefList *child, int lineno);

struct ExtDefList			*Build_ExtDefList(struct ExtDef *child_A, struct ExtDefList *child_B, int lineno);

struct ExtDef				*Build_ExtDef(void *child, void (*func_visit)(void *), int lineno);

struct ExtDef_A				*Build_ExtDef_A(struct Specifier *child_A, struct ExtDecList *child_B);

struct ExtDef_B				*Build_ExtDef_B(struct Specifier *child);

struct ExtDef_C				*Build_ExtDef_C(struct Specifier *child_A, struct FunDec *child_B, struct CompSt *child_C);

struct ExtDecList			*Build_ExtDecList(struct VarDec *child_A, struct ExtDecList *child_B, int lineno);

struct Specifier			*Build_Specifier(void *child, void (*func_visit)(void *), int lineno);

struct Specifier_A			*Build_Specifier_A(DataType child);

struct Specifier_B			*Build_Specifier_B(struct StructSpecifier *child);

struct StructSpecifier		*Build_StructSpecifier(void *child, void (*func_visit)(void *), int lineno);

struct StructSpecifier_A	*Build_StructSpecifier_A(struct OptTag *child_A, struct DefList *child_B);

struct StructSpecifier_B	*Build_StructSpecifier_B(struct Tag *child);

struct OptTag				*Build_OptTag(struct ID *child, int lineno);

struct Tag					*Build_Tag(struct ID *child, int lineno);

struct VarDec				*Build_VarDec(void *child, void (*func_visit)(void *), int lineno);

struct VarDec_A				*Build_VarDec_A(struct ID *child);

struct VarDec_B				*Build_VarDec_B(struct VarDec *child_A, struct TYPE_INT *child_B);

struct FunDec				*Build_FunDec(struct ID *child_A, struct VarList *child_B, int lineno);

struct VarList				*Build_VarList(struct ParamDec *child_A, struct VarList *child_B, int lineno);

struct ParamDec				*Build_ParamDec(struct Specifier *child_A, struct VarDec *child_B, int lineno);

struct CompSt				*Build_CompSt(struct DefList *child_A, struct StmtList *child_B, int lineno);

struct StmtList				*Build_StmtList(struct Stmt *child_A, struct StmtList *child_B, int lineno);

struct Stmt					*Build_Stmt(void *child, void (*func_visit)(void *), int lineno);

struct Stmt_Exp				*Build_Stmt_Exp(struct Exp *child);

struct Stmt_CompSt			*Build_Stmt_CompSt(struct CompSt *child);

struct Stmt_Return			*Build_Stmt_Return(struct Exp *child);

struct Stmt_If				*Build_Stmt_If(struct Exp *child_A, struct Stmt *child_B);

struct Stmt_If_Else			*Build_Stmt_If_Else(struct Exp *child_A, struct Stmt *child_B, struct Stmt *child_C);

struct Stmt_While			*Build_Stmt_While(struct Exp *child_A, struct Stmt *child_B);

struct DefList				*Build_DefList(struct Def *child_A, struct DefList *child_B, int lineno);

struct Def					*Build_Def(struct Specifier *child_A, struct DecList *child_B, int lineno);

struct DecList				*Build_DecList(struct Dec *child_A, struct DecList *child_B, int lineno);

struct Dec					*Build_Dec(struct VarDec *child_A, struct Exp *child_B, int lineno);

struct Exp					*Build_Exp(void *child, void (*func_visit)(void *), int lineno);

struct Exp_Assign			*Build_Exp_Assign(struct Exp *child_A, struct Exp *child_B);

struct Exp_Binary_Rel		*Build_Exp_Binary_Rel(struct Exp *child_A, struct Exp *child_B, BinaryOP_Relop child_C);

struct Exp_Binary_Cal		*Build_Exp_Binary_Cal(struct Exp *child_A, struct Exp *child_B, BinaryOP_Calop child_C);

struct Exp_Unary			*Build_Exp_Unary(struct Exp *child_A, UnaryOP child_B);

struct Exp_Function			*Build_Exp_Function(struct ID *child_A, struct Args *child_B);

struct Exp_Array			*Build_Exp_Array(struct Exp *child_A, struct Exp *child_B);

struct Exp_Attribute		*Build_Exp_Attribute(struct Exp *child_A, struct ID *child_B);

struct Exp_Variable			*Build_Exp_Variable(struct ID *child);

struct TYPE_INT				*Build_TYPE_INT(int child);

struct TYPE_FLOAT			*Build_TYPE_FLOAT(float child);

struct Args					*Build_Args(struct Exp *child_A, struct Args *child_B, int lineno);

struct ID					*Build_ID(char *child);




void Visit_Program(struct Program *v);
void Visit_ExtDefList(struct ExtDefList *v);
void Visit_ExtDef(struct ExtDef *v);
void Visit_ExtDef_A(void *v);
void Visit_ExtDef_B(void *v);
void Visit_ExtDef_C(void *v);
void Visit_ExtDecList(struct ExtDecList *v);
void Visit_Specifier(struct Specifier *v);
void Visit_Specifier_A(void *v);
void Visit_Specifier_B(void *v);
void Visit_StructSpecifier(struct StructSpecifier *v);
void Visit_StructSpecifier_A(void *v);
void Visit_StructSpecifier_B(void *v);
void Visit_OptTag(struct OptTag *v);
void Visit_Tag(struct Tag *v);
void Visit_VarDec(struct VarDec *v);
void Visit_VarDec_A(void *v);
void Visit_VarDec_B(void *v);
void Visit_FunDec(struct FunDec *v);
void Visit_VarList(struct VarList *v);
void Visit_ParamDec(struct ParamDec *v);
void Visit_CompSt(struct CompSt *v);
void Visit_StmtList(struct StmtList *v);
void Visit_Stmt(struct Stmt *v);
void Visit_Stmt_Exp(void *v);
void Visit_Stmt_CompSt(void *v);
void Visit_Stmt_Return(void *v);
void Visit_Stmt_If(void *v);
void Visit_Stmt_If_Else(void *v);
void Visit_Stmt_While(void *v);
void Visit_DefList(struct DefList *v);
void Visit_Def(struct Def *v);
void Visit_DecList(struct DecList *v);
void Visit_Dec(struct Dec *v);
void Visit_Exp(struct Exp *v);
void Visit_Exp_Assign(void *v);
void Visit_Exp_Binary_Rel(void *v);
void Visit_Exp_Binary_Cal(void *v);
void Visit_Exp_Unary(void *v);
void Visit_Exp_Function(void *v);
void Visit_Exp_Array(void *v);
void Visit_Exp_Attribute(void *v);
void Visit_Exp_Variable(void *v);
void Visit_TYPE_INT(void *v);
void Visit_TYPE_FLOAT(void *v);
void Visit_Args(struct Args *v);
void Visit_ID(void *v);
void Visit_DataType(DataType v);
void Visit_BinaryOP_Calop(BinaryOP_Calop v);
void Visit_UnaryOP(UnaryOP v);



#endif
