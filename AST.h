#ifndef AST_H
#define AST_H

//#include "list.h"
#include "const.h"

#define STRING_SIZE		20

int indent;

struct TreeNode
{
	int lineno;
};

int TreeNode_GetLineno(struct TreeNode *);

void TreeNode_SetLineno(struct TreeNode *, int);





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






struct Program				*Build_Program(struct ExtDefList *, int);

struct ExtDefList			*Build_ExtDefList(struct ExtDef *, struct ExtDefList *, int);

struct ExtDef				*Build_ExtDef(void *, void (*)(void *), int);

struct ExtDef_A				*Build_ExtDef_A(struct Specifier *, struct ExtDecList *);

struct ExtDef_B				*Build_ExtDef_B(struct Specifier *);

struct ExtDef_C				*Build_ExtDef_C(struct Specifier *, struct FunDec *, struct CompSt *);

struct ExtDecList			*Build_ExtDecList(struct VarDec *, struct ExtDecList *, int);

struct Specifier			*Build_Specifier(void *, void (*)(void *), int);

struct Specifier_A			*Build_Specifier_A(DataType);

struct Specifier_B			*Build_Specifier_B(struct StructSpecifier *);

struct StructSpecifier		*Build_StructSpecifier(void *, void (*)(void *), int);

struct StructSpecifier_A	*Build_StructSpecifier_A(struct OptTag *, struct DefList *);

struct StructSpecifier_B	*Build_StructSpecifier_B(struct Tag *);

struct OptTag				*Build_OptTag(struct ID *, int);

struct Tag					*Build_Tag(struct ID *, int);

struct VarDec				*Build_VarDec(void *, void (*)(void *), int);

struct VarDec_A				*Build_VarDec_A(struct ID *);

struct VarDec_B				*Build_VarDec_B(struct VarDec *, struct TYPE_INT *);

struct FunDec				*Build_FunDec(struct ID *, struct VarList *, int);

struct VarList				*Build_VarList(struct ParamDec *, struct VarList *, int);

struct ParamDec				*Build_ParamDec(struct Specifier *, struct VarDec *, int);

struct CompSt				*Build_CompSt(struct DefList *, struct StmtList *, int);

struct StmtList				*Build_StmtList(struct Stmt *, struct StmtList *, int);

struct Stmt					*Build_Stmt(void *, void (*)(void *), int);

struct Stmt_Exp				*Build_Stmt_Exp(struct Exp *);

struct Stmt_CompSt			*Build_Stmt_CompSt(struct CompSt *);

struct Stmt_Return			*Build_Stmt_Return(struct Exp *);

struct Stmt_If				*Build_Stmt_If(struct Exp *, struct Stmt *);

struct Stmt_If_Else			*Build_Stmt_If_Else(struct Exp *, struct Stmt *, struct Stmt *);

struct Stmt_While			*Build_Stmt_While(struct Exp *, struct Stmt *);

struct DefList				*Build_DefList(struct Def *, struct DefList *, int);

struct Def					*Build_Def(struct Specifier *, struct DecList *, int);

struct DecList				*Build_DecList(struct Dec *, struct DecList *, int);

struct Dec					*Build_Dec(struct VarDec *, struct Exp *, int);

struct Exp					*Build_Exp(void *, void (*)(void *), int);

struct Exp_Assign			*Build_Exp_Assign(struct Exp *, struct Exp *);

struct Exp_Binary_Rel		*Build_Exp_Binary_Rel(struct Exp *, struct Exp *, BinaryOP_Relop);

struct Exp_Binary_Cal		*Build_Exp_Binary_Cal(struct Exp *, struct Exp *, BinaryOP_Calop);

struct Exp_Unary			*Build_Exp_Unary(struct Exp *, UnaryOP);

struct Exp_Function			*Build_Exp_Function(struct ID *, struct Args *);

struct Exp_Array			*Build_Exp_Array(struct Exp *, struct Exp *);

struct Exp_Attribute		*Build_Exp_Attribute(struct Exp *, struct ID *);

struct Exp_Variable			*Build_Exp_Variable(struct ID *);

struct TYPE_INT				*Build_TYPE_INT(int);

struct TYPE_FLOAT			*Build_TYPE_FLOAT(float);

struct Args					*Build_Args(struct Exp *, struct Args *, int);

struct ID					*Build_ID(char *);




void Visit_Program(struct Program *);
void Visit_ExtDefList(struct ExtDefList *);
void Visit_ExtDef(struct ExtDef *);
void Visit_ExtDef_A(void *);
void Visit_ExtDef_B(void *);
void Visit_ExtDef_C(void *);
void Visit_ExtDecList(struct ExtDecList *);
void Visit_Specifier(struct Specifier *);
void Visit_Specifier_A(void *);
void Visit_Specifier_B(void *);
void Visit_StructSpecifier(struct StructSpecifier *);
void Visit_StructSpecifier_A(void *);
void Visit_StructSpecifier_B(void *);
void Visit_OptTag(struct OptTag *);
void Visit_Tag(struct Tag *);
void Visit_VarDec(struct VarDec *);
void Visit_VarDec_A(void *);
void Visit_VarDec_B(void *);
void Visit_FunDec(struct FunDec *);
void Visit_VarList(struct VarList *);
void Visit_ParamDec(struct ParamDec *);
void Visit_CompSt(struct CompSt *);
void Visit_StmtList(struct StmtList *);
void Visit_Stmt(struct Stmt *);
void Visit_Stmt_Exp(void *);
void Visit_Stmt_CompSt(void *);
void Visit_Stmt_Return(void *);
void Visit_Stmt_If(void *);
void Visit_Stmt_If_Else(void *);
void Visit_Stmt_While(void *);
void Visit_DefList(struct DefList *);
void Visit_Def(struct Def *);
void Visit_DecList(struct DecList *);
void Visit_Dec(struct Dec *);
void Visit_Exp(struct Exp *);
void Visit_Exp_Assign(void *);
void Visit_Exp_Binary_Rel(void *);
void Visit_Exp_Binary_Cal(void *);
void Visit_Exp_Unary(void *);
void Visit_Exp_Function(void *);
void Visit_Exp_Array(void *);
void Visit_Exp_Attribute(void *);
void Visit_Exp_Variable(void *);
void Visit_TYPE_INT(void *);
void Visit_TYPE_FLOAT(void *);
void Visit_Args(struct Args *);
void Visit_ID(void *);
void Visit_DataType(DataType);
void Visit_BinaryOP_Calop(BinaryOP_Calop);
void Visit_UnaryOP(UnaryOP);



#endif
