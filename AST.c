#include "AST.h"

/* TreeNode */

inline int TreeNode_GetLineno(struct TreeNode *ptr)
{
	return ptr -> lineno;
}

inline void TreeNode_SetLineno(struct TreeNode *ptr, int key)
{
	ptr -> lineno = key;	
}



/* Build Function */

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

struct ExtDef *Build_ExtDef(void *child, void (*func_visit)(void *), int lineno)
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


struct Specifier *Build_Specifier(void *child, void (*func_visit)(void *), int lineno)
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


struct StructSpecifier *Build_StructSpecifier(void *child, void (*func_visit)(void *), int lineno)
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


struct VarDec *Build_VarDec(void *child, void (*func_visit)(void *), int lineno)
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

struct Stmt *Build_Stmt(void *child, void (*func_visit)(void *), int lineno)
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

struct Exp *Build_Exp(void *child, void (*func_visit)(void *), int lineno)
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

	printf("YES\n");
	strcpy(ptr -> name, child);
	printf("NO\n");

	return ptr;
}







/* Visit Function */


void Visit_Program(struct Program *v)
{
	if (v == NULL) return;

	PRINT("Program");
	
	Visit_ExtDefList(v -> extdeflist);

}

void Visit_ExtDefList(struct ExtDefList *v)
{
	if (v == NULL) return;

	PRINT("ExtDefList");

	Visit_ExtDef(v -> extdef);
	Visit_ExtDefList(v -> extdeflist);

}

void Visit_ExtDef(struct ExtDef *v)
{
	if (v == NULL) return;

	PRINT("ExtDef");
	
	v -> Visit(v -> next);

}

void Visit_ExtDef_A(void *v)
{
	if (v == NULL) return;
	struct ExtDef_A		*ptr = (struct ExtDef_A *)v;

	Visit_Specifier(ptr -> specifier);
	Visit_ExtDecList(ptr -> extdeflist);
}

void Visit_ExtDef_B(void *v)
{
	if (v == NULL) return;
	struct ExtDef_B		*ptr = (struct ExtDef_B *)v;

	Visit_Specifier(ptr -> specifier);

}

void Visit_ExtDef_C(void *v)
{
	if (v == NULL) return;
	struct ExtDef_C		*ptr = (struct ExtDef_C *)v;

	Visit_Specifier(ptr -> specifier);
	Visit_FunDec(ptr -> fundec);
	Visit_CompSt(ptr -> compst);

}

void Visit_ExtDecList(struct ExtDecList *v)
{
	if (v == NULL) return;

	PRINT("ExtDecList");

	Visit_VarDec(v -> vardec);
	Visit_ExtDecList(v -> extdeclist);
}

void Visit_Specifier(struct Specifier *v)
{
	if (v == NULL) return;

	PRINT("Specifier");

	v -> Visit(v -> next);
}

void Visit_Specifier_A(void *v)
{
	if (v == NULL) return;
	struct Specifier_A	*ptr = (struct Specifier_A *)v;

	PRINT("TYPE");

}

void Visit_Specifier_B(void *v)
{
	if (v == NULL) return;
	struct Specifier_B	*ptr = (struct Specifier_B *)v;

	Visit_StructSpecifier(ptr -> structspecifier);

}

void Visit_StructSpecifier(struct StructSpecifier *v)
{
	if (v == NULL) return;
	PRINT("StructSpecifier");

	v -> Visit(v -> next);

}

void Visit_StructSpecifier_A(void *v)
{
	if (v == NULL) return;
	struct StructSpecifier_A	*ptr = (struct StructSpecifier_A *)v;

	Visit_OptTag(ptr -> opttag);
	Visit_DefList(ptr -> deflist);

}

void Visit_StructSpecifier_B(void *v)
{
	if (v == NULL) return;
	struct StructSpecifier_B	*ptr = (struct StructSpecifier_B *)v;

	Visit_Tag(ptr -> tag);

}

void Visit_OptTag(struct OptTag *v)
{
	if (v == NULL) return;
	PRINT("OptTag");
	
	Visit_ID(v -> id);
}

void Visit_Tag(struct Tag *v)
{
	if (v == NULL) return;
	PRINT("Tag");

	Visit_ID(v -> id);
}

void Visit_VarDec(struct VarDec *v)
{
	if (v == NULL) return;
	PRINT("VarDec");

	v -> Visit(v -> next);
}

void Visit_VarDec_A(void *v)
{
	if (v == NULL) return;
	struct VarDec_A		*ptr = (struct VarDec_A *)v;

	Visit_ID(ptr -> id);
}

void Visit_VarDec_B(void *v)
{
	if (v == NULL) return;
	struct VarDec_B		*ptr = (struct VarDec_B *)v;

	Visit_VarDec(ptr -> vardec);
	Visit_TYPE_INT(ptr -> type_int);
}

void Visit_FunDec(struct FunDec *v)
{
	if (v == NULL) return;

	PRINT("FunDec");

	Visit_ID(v -> id);
	Visit_VarList(v -> varlist);

}
void Visit_VarList(struct VarList *v)
{
	if (v == NULL) return;

	PRINT("VarList");

	Visit_ParamDec(v -> paramdec);
	Visit_VarList(v -> varlist);
}

void Visit_ParamDec(struct ParamDec *v)
{
	if (v == NULL) return;

	PRINT("ParamDec");

	Visit_Specifier(v -> specifier);
	Visit_VarDec(v -> vardec);
}

void Visit_CompSt(struct CompSt *v)
{
	if (v == NULL) return;

	PRINT("CompSt");

	Visit_DefList(v -> deflist);
	Visit_StmtList(v -> stmtlist);
}

void Visit_StmtList(struct StmtList *v)
{
	if (v == NULL) return;

	PRINT("StmtList");

	Visit_Stmt(v -> stmt);
	Visit_StmtList(v -> stmtlist);

}

void Visit_Stmt(struct Stmt *v)
{
	if (v == NULL) return;

	PRINT("Stmt");

	v -> Visit(v -> next);

}

void Visit_Stmt_Exp(void *v)
{
	if (v == NULL) return;
	struct Stmt_Exp		*ptr = (struct Stmt_Exp *)v;

	Visit_Exp(ptr -> exp);
}

void Visit_Stmt_CompSt(void *v)
{
	if (v == NULL) return;
	struct Stmt_CompSt	*ptr = (struct Stmt_CompSt *)v;

	Visit_CompSt(ptr -> compst);
}

void Visit_Stmt_Return(void *v)
{
	if (v == NULL) return;
	struct Stmt_Return	*ptr = (struct Stmt_Return *)v;

	Visit_Exp(ptr -> exp);

}

void Visit_Stmt_If(void *v)
{
	if (v == NULL) return;
	struct Stmt_If		*ptr = (struct Stmt_If *)v;
	
	Visit_Exp(ptr -> exp);
	Visit_Stmt(ptr -> stmt);
}

void Visit_Stmt_If_Else(void *v)
{
	if (v == NULL) return;
	struct Stmt_If_Else		*ptr = (struct Stmt_If_Else *)v;

	Visit_Exp(ptr -> exp);
	Visit_Stmt(ptr -> stmt_if);
	Visit_Stmt(ptr -> stmt_else);
}

void Visit_Stmt_While(void *v)
{
	if (v == NULL) return;
	struct Stmt_While		*ptr = (struct Stmt_While *)v;

	Visit_Exp(ptr -> exp);
	Visit_Stmt(ptr -> stmt);
}

void Visit_DefList(struct DefList *v)
{
	if (v == NULL) return;

	PRINT("DefList");

	Visit_Def(v -> def);
	Visit_DefList(v -> deflist);

}

void Visit_Def(struct Def *v)
{
	if (v == NULL) return;

	PRINT("Def");

	Visit_Specifier(v -> specifier);
	Visit_DecList(v -> declist);
}

void Visit_DecList(struct DecList *v)
{
	if (v == NULL) return;

	PRINT("DecList");

	Visit_Dec(v -> dec);
	Visit_DecList(v -> declist);
}

void Visit_Dec(struct Dec *v)
{
	if (v == NULL) return;

	PRINT("Dec");

	Visit_VarDec(v -> vardec);
	Visit_Exp(v -> exp);
}

void Visit_Exp(struct Exp *v)
{
	if (v == NULL) return;

	PRINT("Exp");

	v -> Visit(v -> next);
}

void Visit_Exp_Assign(void *v)
{
	if (v == NULL) return;
	struct Exp_Assign	*ptr = (struct Exp_Assign *)v;

	Visit_Exp(ptr -> exp_left);
	Visit_Exp(ptr -> exp_right);
}

void Visit_Exp_Binary_Rel(void *v)
{
	if (v == NULL) return;
	struct Exp_Binary_Rel	*ptr = (struct Exp_Binary_Rel *)v;

	Visit_Exp(ptr -> exp_a);
	Visit_Exp(ptr -> exp_b);
}

void Visit_Exp_Binary_Cal(void *v)
{
	if (v == NULL) return;
	struct Exp_Binary_Cal	*ptr = (struct Exp_Binary_Cal *)v;

	Visit_Exp(ptr -> exp_a);
	Visit_Exp(ptr -> exp_b);
}

void Visit_Exp_Unary(void *v)
{
	if (v == NULL) return;
	struct Exp_Unary	*ptr = (struct Exp_Unary *)v;

	Visit_Exp(ptr -> exp);
}

void Visit_Exp_Function(void *v)
{
	if (v == NULL) return;
	struct Exp_Function		*ptr = (struct Exp_Function *)v;

	Visit_ID(ptr -> func);
	Visit_Args(ptr -> args);
}

void Visit_Exp_Array(void *v)
{
	if (v == NULL) return;
	struct Exp_Array		*ptr = (struct Exp_Array *)v;

	Visit_Exp(ptr -> array);
	Visit_Exp(ptr -> index);
}

void Visit_Exp_Attribute(void *v)
{
	if (v == NULL) return;
	struct Exp_Attribute	*ptr = (struct Exp_Attribute *)v;

	Visit_Exp(ptr -> exp);
	Visit_ID(ptr -> attribute);
}

void Visit_Exp_Variable(void *v)
{
	if (v == NULL) return;
	struct Exp_Variable		*ptr = (struct Exp_Variable *)v;

	Visit_ID(ptr -> var);
}

void Visit_TYPE_INT(void *v)
{
	if (v == NULL) return;
	struct TYPE_INT		*ptr = (struct TYPE_INT *)v;
	PRINT("INT");

}

void Visit_TYPE_FLOAT(void *v)
{
	if (v == NULL) return;
	struct TYPE_FLOAT	*ptr = (struct TYPE_FLOAT *)v;
	PRINT("FLOAT");

}

void Visit_Args(struct Args *v)
{
	if (v == NULL) return;

	PRINT("Args");

	Visit_Exp(v -> exp);
	Visit_Args(v -> args);
}

void Visit_ID(void *v)
{
	if (v == NULL) return;
	struct ID		*ptr = (struct ID *)v;
	PRINT("ID");
}


