#include "SemanticChecker.h"
#include "SymbolsTable.h"
#include "const.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct TYPE		*basic_int, *basic_float;

const char		*ErrorMessage[] =
{
	"Accept", // 0
	"Undefined variable", // 1
	"Undefined function", // 2
	"Redefined variable", // 3
	"Redefined function", // 4
	"Incompatible types when assigning", // 5
	"L-value required as left operand of assignment", // 6
	"Operands type mismatched", // 7
	"Return type mismatched", //8
	"Error Parameter", // 9 <--------------
	"Not an array", // 10
	"Not a function", // 11
	"Operands type mistaken in array", // 12 <------------
	"Illegal use of '.'", // 13 <---------
	"Un-existed field", // 14
	"Redefined variable or initialize variable in struct", // 15
	"Duplicate struct", // 16
	"Undefined struct", // 17
	"Function declared but undefined", // 18
	"Function inconsistent between declaration and defination", // 19
	"Not satisfied the assumptions!" // 20
};

int errorline;

void SemanticChecker(int lineno, int error_type, char *message)
{
	++ ErrorCounter;
	if (message != NULL)
		fprintf(stderr, "Error Type %d at line %d: Semantic Error, %s %s\n", error_type, lineno, ErrorMessage[error_type], message);
	else
		fprintf(stderr, "Error Type %d at line %d: Semantic Error, %s\n", error_type, lineno, ErrorMessage[error_type]);
}

void Build_TYPE_Basic(void)
{
	basic_int = (struct TYPE *)malloc(sizeof(struct TYPE));	
	basic_int -> level = Basic;
	basic_int -> basic = TYPE_INT;

	basic_float = (struct TYPE *)malloc(sizeof(struct TYPE));	
	basic_float -> level = Basic;
	basic_float -> basic = TYPE_FLOAT;
}

// YES
struct Parameter	*Build_Parameter(char *name, struct TYPE *type, struct Parameter *next)
{
	struct Parameter	*ptr = (struct Parameter *)malloc(sizeof(struct Parameter));

	strcpy(ptr -> name, name);
	ptr -> type = type;
	ptr -> next = next;

	return ptr;
}

// YES
struct ArrayType	*Build_Array(struct TYPE *type, int size)
{
	struct ArrayType	*ptr = (struct ArrayType *)malloc(sizeof(struct ArrayType));

	ptr -> next = type;
	ptr -> size = size;

	return ptr;
}

// YES
struct StructureType	*Build_Structure(char *name, struct TYPE *type, struct StructureType *next)
{
	struct StructureType	*ptr = (struct StructureType *)malloc(sizeof(struct StructureType));
	strcpy(ptr -> name, name);
	ptr -> type = type;
	ptr -> next = next;

	return ptr;
}

struct VariableAttribute	*Build_VariableAttribute(struct TYPE *type, int vint, float vfloat, unsigned int address, int size)
{
	struct VariableAttribute	*ptr = (struct VariableAttribute *)malloc(sizeof(struct VariableAttribute));
	ptr -> type = type;
	if (type == TYPE_INT)
		ptr -> v_int = vint;
	else
		ptr -> v_float = vfloat;
	ptr -> address = address;
	ptr -> size = size;

	return ptr;
}

struct FunctionAttribute	*Build_FunctionAttribute(struct TYPE *type, struct Parameter *args, unsigned int address, int size)
{
	struct FunctionAttribute	*ptr = (struct FunctionAttribute *)malloc(sizeof(struct FunctionAttribute));

	ptr -> return_type = type;
	ptr -> args = args;
	ptr -> address = address;
	ptr -> size = size;

	return ptr;
}

struct Attribute	*Build_Attribute(AttributeType attr_type, struct FunctionAttribute *func, struct VariableAttribute *var)
{
	struct Attribute	*ptr = (struct Attribute *)malloc(sizeof(struct Attribute));

	ptr -> type = attr_type;

	if (attr_type == Variable)
		ptr -> var = var;
	else if (attr_type == Function)
		ptr -> func = func;
	else
		printf("Fucking!\n");

	return ptr;
}

void Structure_print(struct StructureType *ptr)
{
	printf("{\n");
	for (;ptr; ptr = ptr -> next)
	{
		TYPE_print(ptr -> type);
		printf(": %s\n", ptr -> name);
	}
	printf("}");
}

void TYPE_print(struct TYPE *ptr)
{
	if (ptr -> level == Basic)
	{
		if (ptr -> basic == TYPE_INT)
			printf("Int");
		else if (ptr -> basic == TYPE_FLOAT)
			printf("Float");
		else
			printf("============FUCK=============");
	}
	else if (ptr -> level == Array)
	{
		Array_print(ptr -> array);
	}
	else if (ptr -> level == Structure)
	{
		Structure_print(ptr -> structure);
	}
	else
		printf("============FUCK=============");
}

void Parameter_print(struct Parameter *ptr)
{
	while (ptr)
	{
		printf("\t%s: ", ptr -> name);			
		TYPE_print(ptr -> type);
		printf("\n");
		ptr = ptr -> next;
	}
}

void Function_print(struct FunctionAttribute *ptr)
{
	printf("Return Type: \n");
	TYPE_print(ptr -> return_type);
	printf("\n");

	printf("Parameters: \n");
	Parameter_print(ptr -> args);
	printf("\n");
}

void Attribute_print(struct Attribute *ptr)
{
	if (ptr -> type == Variable)
	{
		printf("variable: ");
		TYPE_print(ptr -> var -> type);
		printf("\n");
	}
	else if (ptr -> type == Function)
	{
		printf("function: ");
		Function_print(ptr -> func);
		printf("\n");
	}
	else
		printf("============FUCK=============");
}

void Array_print(struct ArrayType *ptr)
{
	TYPE_print(ptr -> next);
	printf("[%d]", ptr -> size);
}

void Remove_Structure(struct StructureType *ptr)
{
	if (ptr == NULL) return;
	Remove_Structure(ptr -> next);
	free(ptr);
}

/* ================================================================================= */

void SemanticAnalysis(struct Program *AST)
{
	Build_TYPE_Basic();
	ST_clear(ST);
	ST_clear(SST);
	Scope_clear(&Scope);
	Scope_clear(&StructScope);
	DST_clear();

	SemanticCheck_Program(AST);

	DST_print(DST);
	printf("=================================\n");
	Scope_print(&Scope);
}


// YES
void SemanticCheck_Program(struct Program *root)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	Scope_push(&Scope);

	SemanticCheck_ExtDefList(root -> extdeflist);
}

// YES
void SemanticCheck_ExtDefList(struct ExtDefList *root)
{
	while (root)
	{
		SemanticCheck_ExtDef(root -> extdef);
		root = root -> extdeflist;
	}
}

// YES
void SemanticCheck_ExtDef(struct ExtDef *root)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	root -> SemanticCheck(root -> next);
}

// YES
void SemanticCheck_ExtDef_A(void *root)
{
	struct ExtDef_A		*ptr = (struct ExtDef_A *)root;
	struct TYPE		*type = SemanticCheck_Specifier(ptr -> specifier);
	if (type == NULL)
	{
		//need report error?	
	}
	SemanticCheck_ExtDecList(ptr -> extdeclist, type);
}

// YES
void SemanticCheck_ExtDef_B(void *root)
{
	struct ExtDef_B		*ptr = (struct ExtDef_B *)root;
	SemanticCheck_Specifier(ptr -> specifier);
}

// NO
void SemanticCheck_ExtDef_C(void *root)
{
	struct ExtDef_C		*ptr = (struct ExtDef_C *)root;
	struct TYPE		*return_type = SemanticCheck_Specifier(ptr -> specifier);
	if (return_type == NULL)
	{
		//need report Error?
		return;
	}

	SemanticCheck_FunDec(ptr -> fundec, return_type);
	SemanticCheck_CompSt(ptr -> compst, return_type);
	Scope_pop(&Scope);
	DST_Global();
}

// YES
void SemanticCheck_ExtDecList(struct ExtDecList *root, struct TYPE *type)
{
	while (root)
	{
		SemanticCheck_VarDec(root -> vardec, type);
		root = root -> extdeclist;
	}
}

// YES
struct TYPE *SemanticCheck_Specifier(struct Specifier *root)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	return (root -> SemanticCheck(root -> next));
}

// YES
struct TYPE *SemanticCheck_Specifier_A(void *root)
{
	struct Specifier_A	*ptr = (struct Specifier_A *)root;

	struct TYPE		*ret = (struct TYPE *)malloc(sizeof(struct TYPE));
	ret -> level = Basic;
	ret -> basic = ptr -> type;

	return ret;
}

// YES
struct TYPE *SemanticCheck_Specifier_B(void *root)
{
	struct Specifier_B	*ptr = (struct Specifier_B *)root;

	struct StructureType	*new = SemanticCheck_StructSpecifier(ptr -> structspecifier);

	if (new == NULL) return NULL;

	struct TYPE		*ret = (struct TYPE *)malloc(sizeof(struct TYPE));
	ret -> level = Structure;
	ret -> structure = new;

	return ret;
}

// YES
struct StructureType *SemanticCheck_StructSpecifier(struct StructSpecifier *root)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	return (root -> SemanticCheck(root -> next));
}

// YES
struct StructureType *SemanticCheck_StructSpecifier_A(void *root)
{
	struct StructSpecifier_A	*ptr = (struct StructSpecifier_A *)root;	
	
	int saved_errorline = errorline;

	Scope_push(&StructScope);
	struct StructureType		*struct_type = SemanticCheck_Structure_DefList(ptr -> deflist);
	Scope_pop(&StructScope);

	char *name = SemanticCheck_OptTag(ptr -> opttag);
	if (struct_type != NULL)
	{
		if (name != NULL)
		{
			if (DST_check(DST, struct_type, name))
			{
				DST = DST_insert(DST, name, struct_type);
			}
			else
			{
				SemanticChecker(saved_errorline, 16, name);
			}
		}
	}
	/*
	else
	{
		if (name != NULL)
			printf("error in defination of struct %s\n", name);
		else
			printf("error in void\n");
		//Error!
	}
	*/
	return struct_type;
	
}

// YES
struct StructureType *SemanticCheck_StructSpecifier_B(void *root)
{
	struct StructSpecifier_B	*ptr = (struct StructSpecifier_B *)root;	
	char *name = SemanticCheck_Tag(ptr -> tag);
	struct DynamicStructureTable	*target = DST_find(DST, name);
	if (target != NULL)
	{
		return target -> attr;
	}
	else
	{
		SemanticChecker(errorline, 17, name);
		return NULL;
	}
}

// YES
char *SemanticCheck_OptTag(struct OptTag *root)
{
	if (root == NULL)
		return NULL;
	else
		return (root -> id -> name);
}

// YES
char *SemanticCheck_Tag(struct Tag *root)
{
	return (root -> id -> name);
}

// YES
struct SymbolsTable *SemanticCheck_VarDec(struct VarDec *root, struct TYPE *type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	return (root -> SemanticCheck(root -> next, type));
}

// YES
struct StructureType *SemanticCheck_Structure_VarDec(struct VarDec *root, struct TYPE *type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	return root -> SemanticStructCheck(root -> next, type);
}

struct Parameter	*SemanticCheck_Parameter_VarDec(struct VarDec *root, struct TYPE *type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	return root -> SemanticParameterCheck(root -> next, type);
}

// YES
struct SymbolsTable *SemanticCheck_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(ST, ptr -> id -> name, &Scope))
	{
		SemanticChecker(errorline, 3, ptr -> id -> name);
		return;
	}
	struct SymbolsTable *new = ST_insert(ST, ptr -> id -> name, &Scope);

	new -> attr = Build_Attribute(Variable, NULL, Build_VariableAttribute(type, 0, 0, 0, 0));	// <-----

	return new;
}

// YES
struct StructureType *SemanticCheck_Structure_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(SST, ptr -> id -> name, &StructScope))
	{
		//printf("Element %s is multidefined in struct!\n", ptr -> id -> name);
		SemanticChecker(errorline, 15, ptr -> id -> name);
		return NULL;
	}

	struct SymbolsTable *new = ST_insert(SST, ptr -> id -> name, &StructScope);

	new -> attr = Build_Attribute(Variable, NULL, Build_VariableAttribute(type, 0, 0, 0, 0));	// <-----

	return Build_Structure(ptr -> id -> name, type, NULL);
}

// YES
struct Parameter	*SemanticCheck_Parameter_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(ST, ptr -> id -> name, &Scope))
	{
		//printf("Parameter %s is multidefined in function!\n", ptr -> id -> name);
		SemanticChecker(errorline, 9, ptr -> id -> name);
		return NULL;
	}

	struct SymbolsTable * new = ST_insert(ST, ptr -> id -> name, &Scope);

	new -> attr = Build_Attribute(Variable, NULL, Build_VariableAttribute(type, 0, 0, 0, 0));	// <-----

	return Build_Parameter(ptr -> id -> name, type, NULL);
}

// NO
struct SymbolsTable *SemanticCheck_VarDec_B(void *root, struct TYPE *type)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;

	struct TYPE		*new = (struct TYPE *)malloc(sizeof(struct TYPE));
	new -> level = Array;
	new -> array = Build_Array(type, ptr -> exp_int -> key);

	return SemanticCheck_VarDec(ptr -> vardec, new);
	// Should Delete Type if failed
}

// NO
struct StructureType *SemanticCheck_Structure_VarDec_B(void *root, struct TYPE *type)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;

	struct TYPE		*new = (struct TYPE *)malloc(sizeof(struct TYPE));
	new -> level = Array;
	new -> array = Build_Array(type, ptr -> exp_int -> key);

	return SemanticCheck_Structure_VarDec(ptr -> vardec, new);
	// Should Delete Type if failed
}

// NO
struct Parameter	*SemanticCheck_Parameter_VarDec_B(void *root, struct TYPE *type)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;

	struct TYPE		*new = (struct TYPE *)malloc(sizeof(struct TYPE));
	new -> level = Array;
	new -> array = Build_Array(type, ptr -> exp_int -> key);

	return SemanticCheck_Parameter_VarDec(ptr -> vardec, new);
	// Should Delete Type if failed
}

// YES
void SemanticCheck_FunDec(struct FunDec *root, struct TYPE *return_type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	if (ST_find_scope(ST, root -> id -> name, &Scope))
	{
		//printf("function %s is multidefined!\n", root -> id -> name);
		SemanticChecker(errorline, 4, root -> id -> name);
		Scope_push(&Scope); // Scope_pop in ExtDef_C
		//DST_Local(); <--- useless
		return;
	}

	struct SymbolsTable *func = ST_insert(ST, root -> id -> name, &Scope);

	Scope_push(&Scope); // Scope_pop in ExtDef_C
	DST_Local();

	struct Parameter *args = SemanticCheck_VarList(root -> varlist);

	func -> attr = Build_Attribute(Function, Build_FunctionAttribute(return_type, args, 0, 0), NULL); // <------
}

// YES?
struct Parameter *SemanticCheck_VarList(struct VarList *root)
{
	if (root == NULL) return NULL;

	struct Parameter	*ptr = SemanticCheck_ParamDec(root -> paramdec);
	if (ptr == NULL) return SemanticCheck_VarList(root -> varlist);
	ptr -> next = SemanticCheck_VarList(root -> varlist);
	return ptr;
}

// YES?
struct Parameter *SemanticCheck_ParamDec(struct ParamDec *root)
{
	struct TYPE		*type = SemanticCheck_Specifier(root -> specifier);
	if (type == NULL) return NULL;
	return SemanticCheck_Parameter_VarDec(root -> vardec, type);
}


// YES
void SemanticCheck_CompSt(struct CompSt *root, struct TYPE *return_type)
{
	SemanticCheck_DefList(root -> deflist);
	SemanticCheck_StmtList(root -> stmtlist, return_type);
}



// YES
void SemanticCheck_StmtList(struct StmtList *root, struct TYPE *return_type)
{
	while (root)
	{
		SemanticCheck_Stmt(root -> stmt, return_type);
		root = root -> stmtlist;
	}
}

// YES
void SemanticCheck_Stmt(struct Stmt *root, struct TYPE *return_type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	root -> SemanticCheck(root -> next, return_type);
}


// YES
void SemanticCheck_Stmt_Exp(void *root, struct TYPE *return_type)		// return_type is useless
{
	struct Stmt_Exp		*ptr = (struct Stmt_Exp *)root;

	SemanticCheck_Exp(ptr -> exp);
}

// YES
void SemanticCheck_Stmt_Return(void *root, struct TYPE *return_type)
{
	struct Stmt_Return	*ptr = (struct Stmt_Return *)root;

	struct TYPE		*exp_type = SemanticCheck_Exp(ptr -> exp);
	if (Checking_MatchingType(return_type, exp_type) == 0)
	{
		SemanticChecker(errorline, 8, NULL);
	}
}


// YES
void SemanticCheck_Stmt_CompSt(void *root, struct TYPE *return_type)
{
	struct Stmt_CompSt	*ptr = (struct Stmt_CompSt *)root;
	Scope_push(&Scope);
	SemanticCheck_CompSt(ptr -> compst, return_type);
	Scope_pop(&Scope);
}

// YES
void SemanticCheck_Stmt_If(void *root, struct TYPE *return_type)
{
	struct Stmt_If		*ptr = (struct Stmt_If *)root;
	
	Checking_Condition(ptr -> exp);
	SemanticCheck_Stmt(ptr -> stmt, return_type);
}

// YES
void SemanticCheck_Stmt_If_Else(void *root, struct TYPE *return_type)
{
	struct Stmt_If_Else	*ptr = (struct Stmt_If_Else *)root;

	Checking_Condition(ptr -> exp);
	SemanticCheck_Stmt(ptr -> stmt_if, return_type);
	SemanticCheck_Stmt(ptr -> stmt_else, return_type);
}

// YES
void SemanticCheck_Stmt_While(void *root, struct TYPE *return_type)
{
	struct Stmt_While	*ptr = (struct Stmt_While *)root;

	Checking_Condition(ptr -> exp);
	SemanticCheck_Stmt(ptr -> stmt, return_type);
}


// YES
void SemanticCheck_DefList(struct DefList *root)
{
	while (root)
	{
		SemanticCheck_Def(root -> def);
		root = root -> deflist;
	}
}

// YES?
void SemanticCheck_Def(struct Def *root)
{
	struct TYPE		*type = SemanticCheck_Specifier(root -> specifier);
	if (type == NULL) return;
	SemanticCheck_DecList(root -> declist, type);
}

// YES
void SemanticCheck_DecList(struct DecList *root, struct TYPE *type)
{
	while (root)
	{
		SemanticCheck_Dec(root -> dec, type);
		root = root -> declist;
	}
}

// YES
void SemanticCheck_Dec(struct Dec *root, struct TYPE *type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	if (root -> exp != NULL)
		if (Checking_MatchingType(type, SemanticCheck_Exp(root -> exp)) == 0)
		{
			SemanticChecker(errorline, 5, NULL);
			return;
		}
	SemanticCheck_VarDec(root -> vardec, type);
}


// YES
struct StructureType *SemanticCheck_Structure_DefList(struct DefList *root)
{
	if (root == NULL) return NULL;
	struct StructureType	*head = SemanticCheck_Structure_Def(root -> def);
	if (head == NULL) return SemanticCheck_Structure_DefList(root -> deflist);
	struct StructureType	*tail;
	for (tail = head; tail -> next; tail = tail -> next);

	tail -> next = SemanticCheck_Structure_DefList(root -> deflist);
	return head;
}

// YES
struct StructureType *SemanticCheck_Structure_Def(struct Def *root)
{
	struct TYPE		*type = SemanticCheck_Specifier(root -> specifier);
	if (type == NULL) return NULL;
	return SemanticCheck_Structure_DecList(root -> declist, type);
}

// YES
struct StructureType *SemanticCheck_Structure_DecList(struct DecList *root, struct TYPE *type)
{
	if (root == NULL) return NULL;
	struct StructureType	*ptr = SemanticCheck_Structure_Dec(root -> dec, type);
	if (ptr == NULL) return SemanticCheck_Structure_DecList(root -> declist, type);

	// debug? head tail?
	ptr -> next = SemanticCheck_Structure_DecList(root -> declist, type);
	return ptr;
}

// YES
struct StructureType *SemanticCheck_Structure_Dec(struct Dec *root, struct TYPE *type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	if (root -> exp != NULL)
	{
		//printf("error! initialize in struct\n");
		SemanticChecker(errorline, 15, NULL);
		return NULL;
		//Error!  In struct, couldn't initialize the variable
	}

	return SemanticCheck_Structure_VarDec(root -> vardec, type);
}

// YES
struct TYPE *SemanticCheck_Exp(struct Exp *root)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	return (root -> SemanticCheck(root -> next));
}


// YES
struct TYPE *SemanticCheck_Exp_Assign(void *root)
{
	struct Exp_Assign		*ptr = (struct Exp_Assign *)root;
	struct TYPE		*L = SemanticCheck_Exp(ptr -> exp_left), *R = SemanticCheck_Exp(ptr -> exp_right);

	if (L == NULL || R == NULL) return NULL;

	if (ptr -> exp_left -> left_value == 0)
	{
		SemanticChecker(errorline, 6, NULL);
		return NULL;
	}

	if (Checking_MatchingType(L, R) == 0)
	{
		SemanticChecker(errorline, 5, NULL);
		return NULL;
	}
	return L;
}

// YES
struct TYPE *SemanticCheck_Exp_Binary_Rel(void *root)
{
	struct Exp_Binary_Rel	*ptr = (struct Exp_Binary_Rel *)root;
	struct TYPE		*L = SemanticCheck_Exp(ptr -> exp_a), *R = SemanticCheck_Exp(ptr -> exp_b);

	if (L == NULL || R == NULL) return NULL;

	if (L -> level != Basic || L -> basic != TYPE_INT)
	{
		SemanticChecker(errorline, 20, NULL);
		return NULL;
	}
	if (R -> level != Basic || R -> basic != TYPE_INT)
	{
		SemanticChecker(errorline, 20, NULL);
		return NULL;
	}
	return L;
}

// YES
struct TYPE *SemanticCheck_Exp_Binary_Cal(void *root)
{
	struct Exp_Binary_Cal	*ptr = (struct Exp_Binary_Cal *)root;
	struct TYPE		*L = SemanticCheck_Exp(ptr -> exp_a), *R = SemanticCheck_Exp(ptr -> exp_b);

	if (L == NULL || R == NULL) return NULL;

	if (ptr -> op == OP_AND || ptr -> op == OP_OR)
	{
		if (L -> level != Basic || L -> basic != TYPE_INT)
		{
			SemanticChecker(errorline, 20, NULL);
			return NULL;
		}
		if (R -> level != Basic || R -> basic != TYPE_INT)
		{
			SemanticChecker(errorline, 20, NULL);
			return NULL;
		}
	}
	else
	{
		if (Checking_MatchingType(L, R) == 0)
		{
			//printf("Type Dismatch~\n");
			SemanticChecker(errorline, 7, NULL);
			return NULL;
		}
	}
	return L;
}

// YES?
struct TYPE *SemanticCheck_Exp_Unary(void *root)
{
	struct Exp_Unary	*ptr = (struct Exp_Unary *)root;
	if (ptr -> op == OP_PAR)
		return SemanticCheck_Exp(ptr -> exp);
	else
	{
		struct TYPE		*type = SemanticCheck_Exp(ptr -> exp);
		if (type == NULL) return NULL;
		if (ptr -> op == OP_NEG)
		{
			if (type -> level != Basic)
			{
				SemanticChecker(errorline, 7, NULL);
				return NULL;
			}
		}
		else
		{
			if (type -> level != Basic || type -> basic != TYPE_INT)
			{
				SemanticChecker(errorline, 20, NULL);
				return NULL;
			}
		}
		return type;
	}

}

// YES
struct TYPE *SemanticCheck_Exp_Function(void *root)
{
	struct Exp_Function		*ptr = (struct Exp_Function *)root;
	struct SymbolsTable		*finding = ST_find(ST, ptr -> func -> name);
	if (finding == NULL)
	{
		SemanticChecker(errorline, 2, ptr -> func -> name);
		return NULL;
	}

	assert(!(finding -> attr == NULL));

	if (finding -> attr -> type != Function)
	{
		SemanticChecker(errorline, 11, ptr -> func -> name);
		return NULL;
	}

	struct FunctionAttribute	*attr = finding -> attr -> func;
	if (!Checking_Parameter(ptr -> args, attr -> args))
	{
		SemanticChecker(errorline, 9, NULL);
		return NULL;
	}
	return (attr -> return_type);
} 

struct TYPE *SemanticCheck_Exp_Array(void *root)
{
	struct Exp_Array	*ptr = (struct Exp_Array *)root;
	
	struct TYPE		*index = SemanticCheck_Exp(ptr -> index);
	if (index == NULL) return NULL;

	if (index -> level != Basic || index -> basic != TYPE_INT)
	{
		SemanticChecker(errorline, 12, NULL);
		return NULL;
	}

	struct TYPE		*inner = SemanticCheck_Exp(ptr -> array);
	if (inner == NULL) return NULL;

	if (inner -> level != Array)
	{
		SemanticChecker(errorline, 10, NULL);
		return NULL;
	}

	return (inner -> array -> next);
}

struct TYPE *SemanticCheck_Exp_Attribute(void *root)
{
	struct Exp_Attribute	*ptr = (struct Exp_Attribute *)root;

	struct TYPE		*head = SemanticCheck_Exp(ptr -> exp);
	if (head == NULL) return NULL;

	if (head -> level != Structure)
	{
		SemanticChecker(errorline, 13, NULL);
		return NULL;
	}

	struct StructureType	*entry;
	for (entry = head -> structure; entry != NULL; entry = entry -> next)
		if (strcmp(entry -> name, ptr -> attribute -> name) == 0)		
			return (entry -> type);

	SemanticChecker(errorline, 14, NULL);
	return NULL;
}

struct TYPE *SemanticCheck_Exp_INT(void *root)
{
	return basic_int;	
}

struct TYPE *SemanticCheck_Exp_FLOAT(void *root)
{
	return basic_float;
}

struct TYPE *SemanticCheck_Exp_Variable(void *root)
{
	struct Exp_Variable		*ptr = (struct Exp_Variable *)root;
	struct SymbolsTable		*finding = ST_find(ST, ptr -> var -> name);
	if (finding == NULL)
	{
		SemanticChecker(errorline, 1, ptr -> var -> name);
		return NULL;
	}
	assert(!(finding -> attr == NULL));

	if (finding -> attr -> type != Variable)
	{
		// Not a variable
		SemanticChecker(errorline, 1, ptr -> var -> name);
		return NULL;
	}
	struct VariableAttribute	*attr = finding -> attr -> var;
	return (attr -> type);
	
}

int Checking_Parameter(struct Args *root, struct Parameter *args)
{
	for (; root != NULL && args != NULL; root = root -> args, args = args -> next)
		if (Checking_MatchingType(SemanticCheck_Exp(root -> exp), args -> type) == 0) return 0;
	return (root == NULL && args == NULL ? 1 : 0);
}


void Checking_Condition(struct Exp *exp)
{
	errorline = TreeNode_GetLineno(&(exp -> tree));
	struct TYPE		*type = SemanticCheck_Exp(exp);
	if (type == NULL) return;
	if (Checking_MatchingType(type, basic_int) == 0)
	{
		SemanticChecker(errorline, 20, NULL);
	}
}


int Checking_MatchingType(struct TYPE *typeA, struct TYPE *typeB)		// 1 --> Match, 0 --> Dismatch
{
	if (typeA == NULL && typeB == NULL) return 1;
	if (typeA == NULL || typeB == NULL) return 0;

	if (typeA -> level != typeB -> level) return 0;

	if (typeA -> level == Basic)
		return (typeA -> basic == typeB -> basic ? 1 : 0);
	else if (typeA -> level == Array)
	{
		assert(!(typeA -> array == NULL));
		assert(!(typeB -> array == NULL));
		return (Checking_MatchingType(typeA -> array -> next, typeB -> array -> next));
	}
	else if (typeA -> level == Structure)
	{
		struct StructureType	*L, *R;
		for (L = typeA -> structure, R = typeB -> structure; L != NULL && R != NULL; L = L -> next, R = R -> next)
			if (Checking_MatchingType(L -> type, R -> type) == 0) return 0;
		return (L == NULL && R == NULL ? 1 : 0);
	}
	else
		printf("Fucking~\n");
}

