#ifndef CMM_H
#define CMM_H

#include "const.h"
#include "utility.h"
#include "LexChecker.h"
#include "SyntaxChecker.h"
#include "SymbolsTable.h"

//===================================================
struct ArrayType {
	struct TYPE		*next;
	int				size;
};

struct StructureType {
	char					name[NameSize];
	struct TYPE				*type;
	struct StructureType	*next;
};

struct TYPE {
	enum {
		Basic, Array, Structure//,Error
	} level;

	union {
		DataType				basic;
		struct ArrayType		*array;
		struct StructureType	*structure;
	};
};

struct Argument {
	struct TYPE			*type;
	char				name[NameSize];
	struct Argument		*next;
};

typedef enum {
	Declared,
	Defined
} FuncStatus;

struct FunctionAttribute {
	struct TYPE			*return_type;		
	struct Argument		*args;
	unsigned int		address;
	int					size;
	FuncStatus			status;
};


struct VariableAttribute {
	struct TYPE		*type;
	union
	{
		int			v_int;
		float		v_float;
	};
	unsigned int	address;
	int				size;
	int				no;
	int				is_ptr;
};

// For IR Code
struct StructureAttribute {
	struct StructureType	*type;
	int		size;
};

typedef enum {
	Variable,
	Function,
	Struct		// For IR Code
} AttributeType;

struct Attribute {
	AttributeType					type;
	union {
		struct FunctionAttribute	*func;
		struct VariableAttribute	*var;
		struct StructureAttribute	*structure;		// For IR Code
	};
};


//================================================

typedef enum {
		VARIABLE, CONSTANT, ADDRESS, TEMP
} OperandType;

typedef enum {
	LABEL, FUNCTION, ASSIGN, ADD, SUB, MUL, DIVDIV, ValueAddress, AddressValueL, AddressValueR,
	GOTO, CONDGOTO, RETURNRETURN, DEC, ARG, CALL, PARAM, READ, WRITE
} IRCodeType;

struct Operand {
	OperandType		type;
	union {
		int				var_no;
		int				temp_no;
		int				constant;
		unsigned int	addr;
	};
};

struct IRCode {
	IRCodeType		type;
	union
	{
		int		label;
		char	func[NameSize];

		struct Operand		*single;

		struct {
			char	func[NameSize];
			struct Operand	*ret;
		} call;

		struct {
			struct Operand	*left, *right;
		} lr;

		struct {
			struct Operand	*result, *op1, *op2;
		} binary;

		struct {
			BinaryOP_Relop	op;
			struct Operand	*left, *right;
			int next;
		} condjump;

		struct {
			struct Operand	*x;
			int				size;
		} dec;
	};
};

struct IRChain {
	struct IRCode	*code;
	struct IRChain	*prev, *next;
};

struct ArgsChain {
	struct Operand		*arg;
	struct ArgsChain	*next;
};


//====================================================


struct NextBlock {
	struct CodeBlock	*target;
	struct NextBlock	*next;
};

struct CodeBlock {
	int		label;
	struct IRChain		*code;
	struct NextBlock	*out;
};



//================================================


struct TreeNode {
	int lineno;
};

int TreeNode_GetLineno(struct TreeNode *);

void TreeNode_SetLineno(struct TreeNode *, int);


struct Program {
	struct TreeNode		tree;

	struct ExtDefList	*extdeflist;
};


struct ExtDefList {
	struct TreeNode		tree;

	struct ExtDef		*extdef;
	struct ExtDefList	*extdeflist;
};


struct ExtDef {
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);

	void	(*SemanticCheck)(void *);

	struct IRChain		*(*IR)(void *);
};



struct ExtDef_A {
	struct Specifier	*specifier;
	struct ExtDecList	*extdeclist;
};

struct ExtDef_B {
	struct Specifier	*specifier;
};


struct ExtDef_C {
	struct Specifier	*specifier;
	struct FunDec		*fundec;
	struct CompSt		*compst;
};

struct ExtDef_D {
	struct Specifier	*specifier;
	struct FunDec		*fundec;
};

struct ExtDecList {
	struct TreeNode		tree;

	struct VarDec		*vardec;
	struct ExtDecList	*extdeclist;

};


struct Specifier {
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);

	struct TYPE		*(*SemanticCheck)(void *);
};


struct Specifier_A {
	DataType	type;
};

struct Specifier_B {
	struct StructSpecifier	*structspecifier;
};

struct StructSpecifier {
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);

	struct StructureType	*(*SemanticCheck)(void *);
};

struct StructSpecifier_A {
	struct OptTag		*opttag;
	struct DefList		*deflist;
};

struct StructSpecifier_B {
	struct Tag		*tag;
};

struct OptTag {
	struct TreeNode		tree;

	struct ID			*id;
};

struct Tag {
	struct TreeNode		tree;

	struct ID			*id;
};

struct VarDec {
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);

	struct SymbolsTable		*(*SemanticCheck)(void *, struct TYPE *);

	struct StructureType	*(*SemanticStructCheck)(void *, struct TYPE *);

	struct Argument			*(*SemanticArgumentCheck)(void *, struct TYPE *);

	struct IRChain			*(*IR)(void *, struct Operand *);
};

struct VarDec_A {
	struct ID		*id;
};


struct VarDec_B {
	struct VarDec		*vardec;
	struct Exp_INT		*exp_int;
};


struct FunDec {
	struct TreeNode		tree;

	struct ID			*id;
	struct VarList		*varlist;
};


struct VarList {
	struct TreeNode		tree;

	struct ParamDec		*paramdec;
	struct VarList		*varlist;
};


struct ParamDec {
	struct TreeNode		tree;

	struct Specifier	*specifier;
	struct VarDec		*vardec;
};


struct CompSt {
	struct TreeNode		tree;

	struct DefList		*deflist;
	struct StmtList		*stmtlist;
};


struct StmtList {
	struct TreeNode		tree;

	struct Stmt			*stmt;
	struct StmtList		*stmtlist;
};


struct Stmt {
	struct TreeNode		tree;

	void	*next;

	void	(*Visit)(void *);

	void	(*SemanticCheck)(void *, struct TYPE *);

	struct IRChain *(*IR)(void *);
};

struct Stmt_Exp {
	struct Exp		*exp;
};

struct Stmt_CompSt {
	struct CompSt	*compst;
};

struct Stmt_Return {
	struct Exp		*exp;
};

struct Stmt_If {
	struct Exp		*exp;
	struct Stmt		*stmt;
};


struct Stmt_If_Else {
	struct Exp		*exp;
	struct Stmt		*stmt_if, *stmt_else;
};


struct Stmt_While {
	struct Exp		*exp;
	struct Stmt		*stmt;
};


struct DefList {
	struct TreeNode		tree;

	struct Def			*def;
	struct DefList		*deflist;
};

struct Def {
	struct TreeNode		tree;

	struct Specifier	*specifier;
	struct DecList		*declist;
};

struct DecList {
	struct TreeNode		tree;
	
	struct Dec			*dec;
	struct DecList		*declist;
};

struct Dec {
	struct TreeNode		tree;

	struct VarDec		*vardec;
	struct Exp			*exp;
};

struct Exp {
	struct TreeNode		tree;

	int		left_value;

	void	*next;

	struct TYPE			*exp_type;

	void	(*Visit)(void *);

	struct TYPE		*(*SemanticCheck)(void *);
	
	struct IRChain	*(*IR)(void *, struct Operand *, struct TYPE *, int);

	struct IRChain	*(*IR_Condition)(void *, int, int);
};


struct Exp_Assign {
	struct Exp		*exp_left, *exp_right;
};

struct Exp_Binary_Rel {
	struct Exp		*exp_a, *exp_b;
	BinaryOP_Relop	op;
};

struct Exp_Binary_Cal {
	struct Exp		*exp_a, *exp_b;
	BinaryOP_Calop	op;
};

struct Exp_Unary {
	struct Exp		*exp;
	UnaryOP			op;
};

struct Exp_Function {
	struct ID		*func;
	struct Args		*args;
};

struct Exp_Array {
	struct Exp		*array, *index;
};

struct Exp_Attribute {
	struct Exp		*exp;
	struct ID		*attribute;

};

struct Exp_Variable {
	struct ID		*var;
};

struct Exp_INT {
	int		key;
};

struct Exp_FLOAT {
	float	key;
};

struct Args {
	struct TreeNode		tree;

	struct Exp		*exp;
	struct Args		*args;
};

struct ID {
	char	name[NameSize];
};

//====================================================

#include "AST.h"
#include "SemanticChecker.h"
#include "IR.h"
#include "Optimize.h"

#endif
