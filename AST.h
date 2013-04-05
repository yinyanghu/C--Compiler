#ifndef AST_H
#define AST_H

//#include "list.h"

#define STRING_SIZE		20

typedef enum
{
	OP_EQ, OP_NE, OP_LT, OP_GT, OP_GE, OP_LE
} BinaryOP_Relop;

typedef enum
{
	OP_PLUS, OP_MINUS, OP_STAR, OP_DIV, OP_AND, OP_OR
} BinaryOP_Calop;

typedef enum
{
	OP_NOT, OP_NEG, OP_PAR
} UnaryOP;


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

struct Program *Build_Program(struct ExtDefList *child, int lineno);



struct ExtDefList
{
	struct TreeNode		tree;

	struct ExtDef		*extdef;
	struct ExtDefList	*extdeflist;
};

struct ExtDefList *Build_ExtDefList(struct ExtDef *child_A, struct ExtDefList *child_B, int lineno);

struct ExtDef
{
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(struct ExtDef *);
	/*
	union
	{
		struct ExtDef_A		*A;
		struct ExtDef_B		*B;
		struct ExtDef_C		*C;
	};
	*/
};

struct ExtDef *Build_ExtDef(void *child, void (*func_visit)(struct ExtDef *), int lineno);

struct ExtDef_A
{
	struct Specifier	*specifier;
	struct ExtDecList	*extdeflist;
	/* SEMI */
};

struct ExtDef_A *Build_ExtDef_A(struct Specifier *child_A, struct ExtDecList *child_B);
void Visit_ExtDef_A(struct ExtDef_A *v);


struct ExtDef_B
{
	struct Specifier	*specifier;
	/* SEMI */
};

struct ExtDef_B *Build_ExtDef_B(struct Specifier *child);
void Visit_ExtDef_B(struct ExtDef_B *v);


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

	union
	{
		struct Specifier_A	*A;
		struct Specifier_B	*B;
	};
};

struct Specifier_A
{
	struct Type		*type;
};

struct Specifier_B
{
	struct StructSpecifier	*structspecifier;
};

struct StructSpecifier
{
	struct TreeNode		tree;

	union
	{
		struct StructSpecifier_A	*A;
		struct StructSpecifier_B	*B;
	};
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
	struct ID		*id;
};

struct Tag
{
	struct ID		*id;
};

struct VarDec
{
	struct TreeNode		tree;

	union
	{
		struct VarDec_A		*A;
		struct VarDec_B		*B;
	};
};

struct VarDec_A
{
	struct ID		*id;
};

struct VarDec_B
{
	struct VarDec	*vardec;
	struct ID		*id;
};

struct FunDec
{
	struct TreeNode		*tree;

	struct ID			*id;
	struct VarList		*varlist;
};

struct VarList
{
	struct TreeNode		*tree;

	struct ParamDec		*paramdec;
	struct VarList		*varlist;
};

struct ParamDec
{
	struct TreeNode		*tree;

	struct Specifier	*specifier;
	struct VarDec		*vardec;
};

struct CompSt
{
	struct TreeNode		*tree;

	struct DefList		*deflist;
	struct StmtList		*stmtlist;
};

struct StmtList
{
	struct TreeNode		*tree;

	struct Stmt			*stmt;
	struct StmtList		*stmtlist;
};

struct Stmt
{
	struct TreeNode		*tree;

	union
	{
		struct Stmt_Exp		*stmt_exp;
		struct Stmt_CompSt	*stmt_compst;
		struct Stmt_Return	*stmt_return;
		struct Stmt_If		*stmt_if;
		struct Stmt_If_Else	*stmt_if_else;
		struct Stmt_While	*stmt_while;
	};
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
	struct TreeNode		*tree;

	struct Def			*def;
	struct DefList		*deflist;
};

struct Def
{
	struct TreeNode		*tree;

	struct Specifier	*specifier;
	struct DecList		*declist;
};

struct DecList
{
	struct TreeNode		*tree;
	
	struct Dec			*dec;
	struct DecList		*declist;
};

struct Dec
{
	struct TreeNode		*tree;

	struct VarDec		*vardec;
	struct Exp			*exp;
};

struct Exp
{
	struct TreeNode		*tree;

	union
	{
		struct Exp_Assign				*exp_ass;
		struct Exp_Binary_Relation		*exp_rel;
		struct Exp_Binary_Calculaton	*exp_cal;
		struct Exp_Unary				*exp_unary;
		struct Exp_Function				*exp_func;
		struct Exp_Array				*exp_array;
		struct Exp_Attribute			*exp_attr;
		struct Exp_Variable				*exp_var;
		struct Exp_Int					*exp_int;
		struct Exp_Float				*exp_float;
	};
};


struct Exp_Assign
{
	struct Exp		*exp_left, *exp_right;
};

struct Exp_Binary_Relation
{
	struct Exp		*exp_a, *exp_b;
	BinaryOP_Relop	op;
};

struct Exp_Binary_Calculaton
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

struct Exp_Int
{
	int		key;
};

struct Exp_Float
{
	float	key;
};

struct Args
{
	struct Exp		*exp;
	struct Args		*args;
};

struct ID
{
	char	*name[STRING_SIZE];
};

#endif
