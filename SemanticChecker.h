#ifndef SEMANTICCHECKER_H
#define SEMANTICCHECKER_H

#include "const.h"

extern int ErrorCounter;

struct ArrayType {
	struct TYPE		*next;
	int				size;
};

struct StructureType {
	char	name[NameSize];
	struct TYPE		*type;
	struct StructureType	*next;
};

/*
if (Check_Structure_MultiDefined())
{

}
struct StructureType	*ptr = (struct StructureType *)malloc(sizeof(struct StructureType));
*/

struct TYPE {
	enum {
		Basic, Array, Structure//,Error
	} level;

	union {
		DataType	basic;
		struct ArrayType	*array;
		struct StructureType	*structure;
	};
};

struct Parameter {
	struct TYPE			*type;
	char				name[NameSize];
	struct Parameter	*next;
};

struct FunctionAttribute {
	struct TYPE			*return_type;		
	struct Parameter	*args;
	unsigned int		address;
	int					size;
};


struct VariableAttribute {
	struct TYPE		*type;
	union
	{
		int v_int;
		float v_float;
	};
	unsigned int address;
	int size;
};

typedef enum {
	Variable,
	Function
} AttributeType;

struct Attribute {
	AttributeType type;
	union {
		struct FunctionAttribute	*func;
		struct VariableAttribute	*var;
	};
};

#include "AST.h"


void Structure_print(struct StructureType *);
void TYPE_print(struct TYPE *);
void Array_print(struct ArrayType *);
void Attribute_print(struct Attribute *);

struct ArrayType			*Build_Array(struct TYPE *, int);
struct VariableAttribute	*Build_VariableAttribute(struct TYPE *, int, float, unsigned int, int);
struct StructureType		*Build_Structure(char *, struct TYPE *, struct StructureType *);
struct FunctionAttribute	*Build_FunctionAttribute(struct TYPE *, struct Parameter *, unsigned int, int);
struct Attribute			*Build_Attribute(AttributeType, struct FunctionAttribute *, struct VariableAttribute *);

void Remove_Structure(struct StructureType *);

void SemanticChecker(int, int, char *);
void SemanticAnalysis(struct Program *);

char *SemanticCheck_OptTag(struct OptTag *);
char *SemanticCheck_Tag(struct Tag *);

void SemanticCheck_Program(struct Program *);
void SemanticCheck_ExtDefList(struct ExtDefList *);
void SemanticCheck_ExtDef(struct ExtDef *);
void SemanticCheck_ExtDef_A(void *);
void SemanticCheck_ExtDef_B(void *);
void SemanticCheck_ExtDef_C(void *);
void SemanticCheck_ExtDecList(struct ExtDecList *, struct TYPE *type);

struct TYPE				*SemanticCheck_Specifier(struct Specifier *);
struct TYPE				*SemanticCheck_Specifier_A(void *);
struct TYPE				*SemanticCheck_Specifier_B(void *);

struct StructureType	*SemanticCheck_StructSpecifier(struct StructSpecifier *);
struct StructureType	*SemanticCheck_StructSpecifier_A(void *);
struct StructureType	*SemanticCheck_StructSpecifier_B(void *);

struct SymbolsTable		*SemanticCheck_VarDec(struct VarDec *, struct TYPE *);
struct SymbolsTable		*SemanticCheck_VarDec_A(void *, struct TYPE *);
struct SymbolsTable		*SemanticCheck_VarDec_B(void *, struct TYPE *);

struct StructureType	*SemanticCheck_Structure_VarDec(struct VarDec *, struct TYPE *);
struct StructureType	*SemanticCheck_Structure_VarDec_A(void *, struct TYPE *);
struct StructureType	*SemanticCheck_Structure_VarDec_B(void *, struct TYPE *);

struct Parameter		*SemanticCheck_Parameter_VarDec(struct VarDec *, struct TYPE *);
struct Parameter		*SemanticCheck_Parameter_VarDec_A(void *, struct TYPE *);
struct Parameter		*SemanticCheck_Parameter_VarDec_B(void *, struct TYPE *);

struct StructureType	*SemanticCheck_Structure_DefList(struct DefList *);
struct StructureType	*SemanticCheck_Structure_Def(struct Def *);
struct StructureType	*SemanticCheck_Structure_DecList(struct DecList *, struct TYPE *);
struct StructureType	*SemanticCheck_Structure_Dec(struct Dec *, struct TYPE *);

void					SemanticCheck_DefList(struct DefList *);
void					SemanticCheck_Def(struct Def *);
void					SemanticCheck_DecList(struct DecList *, struct TYPE *);
void					SemanticCheck_Dec(struct Dec *, struct TYPE *);


void					SemanticCheck_FunDec(struct FunDec *, struct TYPE *);
struct Parameter		*SemanticCheck_VarList(struct VarList *);
struct Parameter		*SemanticCheck_ParamDec(struct ParamDec *);


void SemanticCheck_StmtList(struct StmtList *, struct TYPE *);
void SemanticCheck_Stmt(struct Stmt *, struct TYPE *);

void SemanticCheck_Stmt_Return(void *, struct TYPE *);
void SemanticCheck_Stmt_CompSt(void *, struct TYPE *);
void SemanticCheck_Stmt_If(void *, struct TYPE *);
void SemanticCheck_Stmt_If_Else(void *, struct TYPE *);
void SemanticCheck_Stmt_While(void *, struct TYPE *);



void SemanticCheck_CompSt(struct CompSt *, struct TYPE *);


void SemanticCheck_Exp(struct Exp *);
void SemanticCheck_Args(struct Args *);

void Checking_Return(struct TYPE *, struct Exp *);
void Checking_Condition(struct Exp *);


#endif
