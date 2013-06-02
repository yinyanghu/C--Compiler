#ifndef SEMANTICCHECKER_H
#define SEMANTICCHECKER_H

extern int ErrorCounter;

void SemanticChecker(int, int);
void SemanticAnalysis(struct Program *);

void Structure_print(struct StructureType *);
void TYPE_print(struct TYPE *);
void Array_print(struct ArrayType *);
void Attribute_print(struct Attribute *);
void Function_print(struct FunctionAttribute *);
void Argument_print(struct Argument *);

void						Build_TYPE_Basic(void);
struct Argument				*Build_Argument(char *, struct TYPE *, struct Argument *);
struct ArrayType			*Build_Array(struct TYPE *, int);
struct VariableAttribute	*Build_VariableAttribute(struct TYPE *, int, float, unsigned int, int, int);
struct StructureType		*Build_Structure(char *, struct TYPE *, struct StructureType *);
struct FunctionAttribute	*Build_FunctionAttribute(struct TYPE *, struct Argument *, unsigned int, int, FuncStatus);
struct StructureAttribute	*Build_StructureAttribute(struct StructureType *, int);
struct Attribute			*Build_Attribute(AttributeType, struct FunctionAttribute *, struct VariableAttribute *, struct StructureAttribute *);

void Remove_Structure(struct StructureType *);

char *SemanticCheck_OptTag(struct OptTag *);
char *SemanticCheck_Tag(struct Tag *);

void SemanticCheck_Program(struct Program *);
void SemanticCheck_ExtDefList(struct ExtDefList *);
void SemanticCheck_ExtDef(struct ExtDef *);
void SemanticCheck_ExtDef_A(void *);
void SemanticCheck_ExtDef_B(void *);
void SemanticCheck_ExtDef_C(void *);
void SemanticCheck_ExtDef_D(void *);
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

struct Argument			*SemanticCheck_Argument_VarDec(struct VarDec *, struct TYPE *);
struct Argument			*SemanticCheck_Argument_VarDec_A(void *, struct TYPE *);
struct Argument			*SemanticCheck_Argument_VarDec_B(void *, struct TYPE *);

struct StructureType	*SemanticCheck_Structure_DefList(struct DefList *);
struct StructureType	*SemanticCheck_Structure_Def(struct Def *);
struct StructureType	*SemanticCheck_Structure_DecList(struct DecList *, struct TYPE *);
struct StructureType	*SemanticCheck_Structure_Dec(struct Dec *, struct TYPE *);

void					SemanticCheck_DefList(struct DefList *);
void					SemanticCheck_Def(struct Def *);
void					SemanticCheck_DecList(struct DecList *, struct TYPE *);
void					SemanticCheck_Dec(struct Dec *, struct TYPE *);


int						SemanticCheck_FunDec(struct FunDec *, struct TYPE *, FuncStatus);
struct Argument			*SemanticCheck_VarList(struct VarList *);
struct Argument			*SemanticCheck_ParamDec(struct ParamDec *);


void SemanticCheck_CompSt(struct CompSt *, struct TYPE *);
void SemanticCheck_StmtList(struct StmtList *, struct TYPE *);
void SemanticCheck_Stmt(struct Stmt *, struct TYPE *);
void SemanticCheck_Stmt_Exp(void *, struct TYPE *);
void SemanticCheck_Stmt_Return(void *, struct TYPE *);
void SemanticCheck_Stmt_CompSt(void *, struct TYPE *);
void SemanticCheck_Stmt_If(void *, struct TYPE *);
void SemanticCheck_Stmt_If_Else(void *, struct TYPE *);
void SemanticCheck_Stmt_While(void *, struct TYPE *);


struct TYPE		*SemanticCheck_Exp(struct Exp *);
struct TYPE		*SemanticCheck_Exp_Assign(void *);
struct TYPE		*SemanticCheck_Exp_Binary_Rel(void *);
struct TYPE		*SemanticCheck_Exp_Binary_Cal(void *);
struct TYPE		*SemanticCheck_Exp_Unary(void *);
struct TYPE		*SemanticCheck_Exp_Function(void *);
struct TYPE		*SemanticCheck_Exp_Array(void *);
struct TYPE		*SemanticCheck_Exp_Attribute(void *);
struct TYPE		*SemanticCheck_Exp_INT(void *);
struct TYPE		*SemanticCheck_Exp_FLOAT(void *);
struct TYPE		*SemanticCheck_Exp_Variable(void *);

void	Checking_DFL(void);
int		Checking_Argument_Args(struct Args *, struct Argument *);
int		Checking_Argument(struct Argument *, struct Argument *);
void	Checking_Condition(struct Exp *);
int		Checking_MatchingType(struct TYPE *, struct TYPE *);
int		Checking_Function(struct FunctionAttribute *, struct FunctionAttribute *);


#endif
