#ifndef CONST_H
#define CONST_H

#define NULL	((void *)0)

#define RED		"\E[1;31m"
#define BLUD	"\E[1;34m"
#define GREEN	"\E[1;32m"
#define PURPLE	"\E[1;35m"
#define NORMAL	"\E[m"

//#define GLOBAL_COLOR

typedef enum
{
	VarUndefined,				// 1
	FuncUndefined,				// 2
	VarMultiDefined,			// 3
	FuncMultiDefined,			// 4
	ExpMismatch,				// 5
	LeftValueError,				// 6
	OpTypeMismatch,				// 7
	ReturnMismatch,				// 8
	FuncParameterMismatch,		// 9
	ArrayOpError,				// 10
	FuncCallOpError,			// 11
	ArrayAccessError,			// 12
	StructAttributeError,		// 13
	AccessStructUndefinedField,	// 14
	StructDefineError,			// 15
	StructMultiDefined,			// 16
	StructUndefined,			// 17
	FuncDeclareButUndefined,	// 18
	FuncDefineConflict			// 19
} SemanticErrorType;

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

typedef enum
{
	TYPE_INT, TYPE_FLOAT
} DataType;

#endif

