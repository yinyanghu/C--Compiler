#include "AST.h"

inline int TreeNode_GetLineno(struct TreeNode *ptr)
{
	return ptr -> lineno;
}

inline void TreeNode_SetLineno(struct TreeNode *ptr, int key)
{
	ptr -> lineno = key;	
}

struct Program *Build_Program(struct ExtDefList *child, int lineno)
{
	struct Program	*ptr = (struct Program *)malloc(sizeof(struct Program));	

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> extdeflist = child;
	
	return ptr;
}

struct ExtDefList *Build_ExtDefList(struct ExtDef *child_A, struct ExtDefList *child_B, int lineno)
{
	struct ExtDefList	*ptr = (struct ExtDefList *)malloc(sizeof(struct ExtDefList));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> extdef = child_A;
	ptr -> extdeflist = child_B;

	return ptr;
}

struct ExtDef *Build_ExtDef(void *child, void (*func_visit)(struct ExtDef *), int lineno)
{
	struct ExtDef	*ptr = (struct ExtDef *)malloc(sizeof(struct ExtDef));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> next = child;
	ptr -> Visit = func_visit;

	return ptr;
}

struct ExtDef_A *Build_ExtDef_A(struct Specifier *child_A, struct ExtDecList *child_B)
{
	struct ExtDef_A		*ptr = (struct ExtDef_A *)malloc(sizeof(struct ExtDef_A));

	ptr -> specifier = child_A;
	ptr -> extdeflist = child_B;

	return ptr;
}

struct ExtDef_B *Build_ExtDef_B(struct Specifier *child)
{
	struct ExtDef_B		*ptr = (struct ExtDef_B *)malloc(sizeof(struct ExtDef_B));

	ptr -> specifier = child;

	return ptr;
}


struct ExtDef_C *Build_ExtDef_C(struct Specifier *child_A, struct FunDec *child_B, struct CompSt *child_C)
{
	struct ExtDef_C		*ptr = (struct ExtDef_C *)malloc(sizeof(struct ExtDef_C));

	ptr -> specifier = child_A;
	ptr -> fundec = child_B;
	ptr -> compst = child_C;

	return ptr;
}

struct ExtDecList *Build_ExtDecList(struct VarDec *child_A, struct ExtDecList *child_B, int lineno)
{
	struct ExtDecList	*ptr = (struct ExtDecList *)malloc(sizeof(struct ExtDecList));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> vardec = child_A;
	ptr -> extdeclist = child_B;

	return ptr;
}


struct Specifier *Build_Specifier(void *child, void (*func_visit)(struct Specifier *), int lineno)
{
	struct Specifier	*ptr = (struct Specifier *)malloc(sizeof(struct Specifier));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> next = child;
	ptr -> Visit = func_visit;

	return ptr;
}

struct Specifier_A *Build_Specifier_A(DataType child)
{
	struct Specifier_A	*ptr = (struct Specifier_A *)malloc(sizeof(struct Specifier_A));	

	ptr -> type = child;

	return ptr;
}


struct Specifier_B *Build_Specifier_B(struct StructSpecifier *child)
{
	struct Specifier_B	*ptr = (struct Specifier_B *)malloc(sizeof(struct Specifier_B));	

	ptr -> structspecifier = child;

	return ptr;
}


struct StructSpecifier *Build_StructSpecifier(void *child, void (*func_visit)(struct StructSpecifier *), int lineno)
{
	struct StructSpecifier	*ptr = (struct StructSpecifier *)malloc(sizeof(struct StructSpecifier));

	TreeNode_SetLineno(&(ptr -> tree), lineno);

	ptr -> next = child;
	ptr -> Visit = func_visit;

	return ptr;
}

struct StructSpecifier_A *Build_StructSpecifier_A(struct OptTag *child_A, struct DefList *child_B)
{
	struct StructSpecifier_A	*ptr = (struct StructSpecifier_A *)malloc(sizeof(struct StructSpecifier_A));

	ptr -> opttag = child_A;
	ptr -> deflist = child_B;

	return ptr;
}

struct StructSpecifier_B *Build_StructSpecifier_B(struct Tag *child)
{

	struct StructSpecifier_B	*ptr = (struct StructSpecifier_B *)malloc(sizeof(struct StructSpecifier_B));

	ptr -> tag = child;

	return ptr;
}

struct OptTag *Build_OptTag(struct ID *child, int lineno)
{
	struct OptTag	*ptr = (struct OptTag *)malloc(sizeof(struct OptTag));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> id = child;

	return ptr;
}

struct Tag *Build_Tag(struct ID *child, int lineno)
{

	struct Tag	*ptr = (struct Tag *)malloc(sizeof(struct Tag));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> id = child;

	return ptr;
}


struct VarDec *Build_VarDec(void *child, void (*func_visit)(struct VarDec *), int lineno)
{
	struct VarDec	*ptr = (struct VarDec *)malloc(sizeof(struct VarDec));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> next = child;
	ptr -> Visit = func_visit;

	return ptr;
}


struct VarDec_A *Build_VarDec_A(struct ID *child)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)malloc(sizeof(struct VarDec_A));

	ptr -> id = child;

	return ptr;
}

struct VarDec_B *Build_VarDec_B(struct VarDec *child_A, struct TYPE_INT *child_B)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)malloc(sizeof(struct VarDec_B));

	ptr -> vardec = child_A;
	ptr -> type_int = child_B;

	return ptr;
}

struct FunDec *Build_FunDec(struct ID *child_A, struct VarList *child_B, int lineno)
{
	struct FunDec		*ptr = (struct FunDec *)malloc(sizeof(struct FunDec));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> id = child_A;
	ptr -> varlist = child_B;

	return ptr;
}

struct VarList *Build_VarList(struct ParamDec *child_A, struct VarList *child_B, int lineno)
{
	struct VarList		*ptr = (struct VarList *)malloc(sizeof(struct VarList));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> paramdec = child_A;
	ptr -> varlist = child_B;

	return ptr;
}

struct ParamDec *Build_ParamDec(struct Specifier *child_A, struct VarDec *child_B, int lineno)
{
	struct ParamDec		*ptr = (struct ParamDec *)malloc(sizeof(struct ParamDec));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> specifier = child_A;
	ptr -> vardec = child_B;

	return ptr;
}

struct CompSt *Build_CompSt(struct DefList *child_A, struct StmtList *child_B, int lineno)
{
	struct CompSt		*ptr = (struct CompSt *)malloc(sizeof(struct CompSt));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> deflist = child_A;
	ptr -> stmtlist = child_B;

	return ptr;
}

struct StmtList *Build_StmtList(struct Stmt *child_A, struct StmtList *child_B, int lineno)
{
	struct StmtList		*ptr = (struct StmtList *)malloc(sizeof(struct StmtList));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> stmt = child_A;
	ptr -> stmtlist = child_B;

	return ptr;
}

struct Stmt *Build_Stmt(void *child, void (*func_visit)(struct Stmt *), int lineno)
{
	struct Stmt		*ptr = (struct Stmt *)malloc(sizeof(struct Stmt));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> next = child;
	ptr -> Visit = func_visit;

	return ptr;
}

struct Stmt_Exp *Build_Stmt_Exp(struct Exp *child)
{
	struct Stmt_Exp		*ptr = (struct Stmt_Exp *)malloc(sizeof(struct Stmt_Exp));

	ptr -> exp = child;

	return ptr;
}

struct Stmt_CompSt *Build_Stmt_CompSt(struct CompSt *child)
{
	struct Stmt_CompSt		*ptr = (struct Stmt_CompSt *)malloc(sizeof(struct Stmt_CompSt));

	ptr -> compst = child;

	return ptr;
}

struct Stmt_Return *Build_Stmt_Return(struct Exp *child)
{
	struct Stmt_Return		*ptr = (struct Stmt_Return *)malloc(sizeof(struct Stmt_Return));

	ptr -> exp = child;

	return ptr;
}
struct Stmt_If *Build_Stmt_If(struct Exp *child_A, struct Stmt *child_B)
{
	struct Stmt_If		*ptr = (struct Stmt_If *)malloc(sizeof(struct Stmt_If));

	ptr -> exp = child_A;
	ptr -> stmt = child_B;

	return ptr;
}

struct Stmt_If_Else *Build_Stmt_If_Else(struct Exp *child_A, struct Stmt *child_B, struct Stmt *child_C)
{
	struct Stmt_If_Else		*ptr = (struct Stmt_If_Else *)malloc(sizeof(struct Stmt_If_Else));

	ptr -> exp = child_A;
	ptr -> stmt_if = child_B;
	ptr -> stmt_else = child_C;

	return ptr;
}

struct Stmt_While *Build_Stmt_While(struct Exp *child_A, struct Stmt *child_B)
{
	struct Stmt_While	*ptr = (struct Stmt_While *)malloc(sizeof(struct Stmt_While));

	ptr -> exp = child_A;
	ptr -> stmt = child_B;

	return ptr;
}

struct DefList *Build_DefList(struct Def *child_A, struct DefList *child_B, int lineno)
{
	struct DefList		*ptr = (struct DefList *)malloc(sizeof(struct DefList));

	TreeNode_SetLineno(&(ptr -> tree), lineno);

	ptr -> def = child_A;
	ptr -> deflist = child_B;

	return ptr;
}

struct Def *Build_Def(struct Specifier *child_A, struct DecList *child_B, int lineno)
{
	struct Def		*ptr = (struct Def *)malloc(sizeof(struct Def));

	TreeNode_SetLineno(&(ptr -> tree), lineno);

	ptr -> specifier = child_A;
	ptr -> declist = child_B;

	return ptr;
}

struct DecList *Build_DecList(struct Dec *child_A, struct DecList *child_B, int lineno)
{
	struct DecList	*ptr = (struct DecList *)malloc(sizeof(struct DecList));

	TreeNode_SetLineno(&(ptr -> tree), lineno);

	ptr -> dec = child_A;
	ptr -> declist = child_B;

	return ptr;
}

struct Dec *Build_Dec(struct VarDec *child_A, struct Exp *child_B, int lineno)
{
	struct Dec		*ptr = (struct Dec *)malloc(sizeof(struct Dec));

	TreeNode_SetLineno(&(ptr -> tree), lineno);

	ptr -> vardec = child_A;
	ptr -> exp = child_B;

	return ptr;
}

struct Exp *Build_Exp(void *child, void (*func_visit)(struct Exp *), int lineno)
{
	struct Exp		*ptr = (struct Exp *)malloc(sizeof(struct Exp));

	TreeNode_SetLineno(&(ptr -> tree), lineno);

	ptr -> next = child;
	ptr -> Visit = func_visit;

	return ptr;
}

struct Exp_Assign *Build_Exp_Assign(struct Exp *child_A, struct Exp *child_B)
{
	struct Exp_Assign		*ptr = (struct Exp_Assign *)malloc(sizeof(struct Exp_Assign));

	ptr -> exp_left = child_A;
	ptr -> exp_right = child_B;

	return ptr;
}

struct Exp_Binary_Rel *Build_Exp_Binary_Rel(struct Exp *child_A, struct Exp *child_B, BinaryOP_Relop child_C)
{
	struct Exp_Binary_Rel	*ptr = (struct Exp_Binary_Rel *)malloc(sizeof(struct Exp_Binary_Rel));

	ptr -> exp_a = child_A;
	ptr -> exp_b = child_B;
	ptr -> op = child_C;

	return ptr;
}

struct Exp_Binary_Cal *Build_Exp_Binary_Cal(struct Exp *child_A, struct Exp *child_B, BinaryOP_Calop child_C)
{
	struct Exp_Binary_Cal	*ptr = (struct Exp_Binary_Cal *)malloc(sizeof(struct Exp_Binary_Cal));

	ptr -> exp_a = child_A;
	ptr -> exp_b = child_B;
	ptr -> op = child_C;

	return ptr;
}

struct Exp_Unary *Build_Exp_Unary(struct Exp *child_A, UnaryOP child_B)
{
	struct Exp_Unary		*ptr = (struct Exp_Unary *)malloc(sizeof(struct Exp_Unary));

	ptr -> exp = child_A;
	ptr -> op = child_B;

	return ptr;
}

struct Exp_Function *Build_Exp_Function(struct ID *child_A, struct Args *child_B)
{
	struct Exp_Function		*ptr = (struct Exp_Function *)malloc(sizeof(struct Exp_Function));
	
	ptr -> func = child_A;
	ptr -> args = child_B;

	return ptr;
}

struct Exp_Array *Build_Exp_Array(struct Exp *child_A, struct Exp *child_B)
{
	struct Exp_Array		*ptr = (struct Exp_Array *)malloc(sizeof(struct Exp_Array));

	ptr -> array = child_A;
	ptr -> index = child_B;

	return ptr;
}

struct Exp_Attribute *Build_Exp_Attribute(struct Exp *child_A, struct ID *child_B)
{
	struct Exp_Attribute	*ptr = (struct Exp_Attribute *)malloc(sizeof(struct Exp_Attribute));

	ptr -> exp = child_A;
	ptr -> attribute = child_B;

	return ptr;
}

struct Exp_Variable *Build_Exp_Variable(struct ID *child)
{
	struct Exp_Variable		*ptr = (struct Exp_Variable *)malloc(sizeof(struct Exp_Variable));

	ptr -> var = child;

	return ptr;
}

struct TYPE_INT *Build_TYPE_INT(int child)
{
	struct TYPE_INT		*ptr = (struct TYPE_INT *)malloc(sizeof(struct TYPE_INT));

	ptr -> key = child;

	return ptr;
}

struct TYPE_FLOAT *Build_TYPE_FLOAT(float child)
{
	struct TYPE_FLOAT	*ptr = (struct TYPE_FLOAT *)malloc(sizeof(struct TYPE_FLOAT));

	ptr -> key = child;

	return ptr;
}

struct Args *Build_Args(struct Exp *child_A, struct Args *child_B, int lineno)
{
	struct Args		*ptr = (struct Args *)malloc(sizeof(struct Args));

	TreeNode_SetLineno(&(ptr -> tree), lineno);
	ptr -> exp = child_A;
	ptr -> args = child_B;

	return ptr;
}

struct ID *Build_ID(char *child)
{
	struct ID		*ptr = (struct ID *)malloc(sizeof(struct ID));

	strcpy(ptr -> name, child);

	return ptr;
}


