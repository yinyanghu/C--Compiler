#include "SemanticChecker.h"
#include "SymbolsTable.h"
#include "const.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void SemanticChecker(int lineno, int error_type, char *message)
{
	++ ErrorCounter;
	if (message != NULL)
		fprintf(stderr, "Error Type %d at line %d: Semantic Error, %s\n", error_type, lineno, message);
	else
		fprintf(stderr, "Error Type %d at line %d: Semantic Error\n", error_type, lineno);
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


// YES?
void SemanticCheck_Program(struct Program *root)
{
	Scope_push(&Scope);
	SemanticCheck_ExtDefList(root -> extdeflist);
}

// YES?
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
		//need report error?
		
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
	return (root -> SemanticCheck(root -> next));
}

// YES
struct StructureType *SemanticCheck_StructSpecifier_A(void *root)	// NULl --> Error
{
	struct StructSpecifier_A	*ptr = (struct StructSpecifier_A *)root;	

	Scope_push(&StructScope);
	struct StructureType		*struct_type = SemanticCheck_Structure_DefList(ptr -> deflist);
	Scope_pop(&StructScope);

	char *name = SemanticCheck_OptTag(ptr -> opttag);
	if (struct_type != NULL)
	{
		//printf("%s\n", name);
		if (name != NULL)
		{
			if (DST_check(DST, struct_type, name))
			{
				DST = DST_insert(DST, name, struct_type);
			}
			else
			{
				printf("struct %s is multidefined!\n", name);
			}
		}
	}
	else
	{
		if (name != NULL)
			printf("error in defination of struct %s\n", name);
		else
			printf("error in void\n");
		//Error!
	}
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
		printf("struct %s is undefined!\n", name);
		//Error!
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

//YES
struct SymbolsTable *SemanticCheck_VarDec(struct VarDec *root, struct TYPE *type)
{
	return (root -> SemanticCheck(root -> next, type));
}

// YES
struct StructureType *SemanticCheck_Structure_VarDec(struct VarDec *root, struct TYPE *type)
{
	return root -> SemanticStructCheck(root -> next, type);
}

struct Parameter	*SemanticCheck_Parameter_VarDec(struct VarDec *root, struct TYPE *type)
{
	return root -> SemanticParameterCheck(root -> next, type);
}

// YES
struct SymbolsTable *SemanticCheck_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(ST, ptr -> id -> name, &Scope))
	{
		printf("%s is multidefined!\n", ptr -> id -> name);
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
		//error
		printf("Element %s is multidefined in struct!\n", ptr -> id -> name);
		return NULL;
	}

	ST_insert(SST, ptr -> id -> name, &StructScope);

	return Build_Structure(ptr -> id -> name, type, NULL);
}

// YES?
struct Parameter	*SemanticCheck_Parameter_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(ST, ptr -> id -> name, &Scope))
	{
		printf("Parameter %s is multidefined in function!\n", ptr -> id -> name);
		return NULL;
	}

	ST_insert(ST, ptr -> id -> name, &Scope);

	return Build_Parameter(ptr -> id -> name, type, NULL);
}

// NO
struct SymbolsTable *SemanticCheck_VarDec_B(void *root, struct TYPE *type)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;

	struct TYPE		*new = (struct TYPE *)malloc(sizeof(struct TYPE));
	new -> level = Array;
	new -> array = Build_Array(type, ptr -> type_int -> key);

	return SemanticCheck_VarDec(ptr -> vardec, new);
	// Should Delete Type if failed
}

// NO
struct StructureType *SemanticCheck_Structure_VarDec_B(void *root, struct TYPE *type)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;

	struct TYPE		*new = (struct TYPE *)malloc(sizeof(struct TYPE));
	new -> level = Array;
	new -> array = Build_Array(type, ptr -> type_int -> key);

	return SemanticCheck_Structure_VarDec(ptr -> vardec, new);
	// Should Delete Type if failed
}

// NO
struct Parameter	*SemanticCheck_Parameter_VarDec_B(void *root, struct TYPE *type)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;

	struct TYPE		*new = (struct TYPE *)malloc(sizeof(struct TYPE));
	new -> level = Array;
	new -> array = Build_Array(type, ptr -> type_int -> key);

	return SemanticCheck_Parameter_VarDec(ptr -> vardec, new);
	// Should Delete Type if failed
}

// YES
void SemanticCheck_FunDec(struct FunDec *root, struct TYPE *return_type)
{
	// if Check_Function()
	if (ST_find_scope(ST, root -> id -> name, &Scope))
	{
		printf("function %s is multidefined!\n", root -> id -> name);
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
	if (type == NULL) return NULL;	//Error
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
	root -> SemanticCheck(root -> next, return_type);
}



// YES
void SemanticCheck_Stmt_Return(void *root, struct TYPE *return_type)
{
	struct Stmt_Return	*ptr = (struct Stmt_Return *)root;
	Checking_Return(return_type, ptr -> exp);
	//type comparison: return_type vs. exp
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
	if (type == NULL) return; //Error
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

// NO
void SemanticCheck_Dec(struct Dec *root, struct TYPE *type)
{
	if (root -> exp == NULL)
	{
		SemanticCheck_VarDec(root -> vardec, type);
	}
	else
	{
			
	}
}


// YES --- not sure!
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

// YES?
struct StructureType *SemanticCheck_Structure_Def(struct Def *root)
{
	struct TYPE		*type = SemanticCheck_Specifier(root -> specifier);
	if (type == NULL) return NULL;	//Error
	return SemanticCheck_Structure_DecList(root -> declist, type);
}

// YES --- not sure!
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
	
	if (root -> exp != NULL)
	{
		return NULL;
		printf("error! initialize in struct\n");
		//Error!  In struct, couldn't initialize the variable
	}

	return SemanticCheck_Structure_VarDec(root -> vardec, type);
}

void SemanticCheck_Exp(struct Exp *root)
{

}

void SemanticCheck_Args(struct Args *root)
{

}

void Checking_Return(struct TYPE *return_type, struct Exp *exp)
{

}

void Checking_Condition(struct Exp *exp)
{

}

int Checking_MatchingType(struct TYPE *typeA, struct TYPE *typeB)
{
	if (typeA == NULL || typeB == NULL)
		printf("Fucking~\n");
	if (typeA -> level != typeB -> level) return 0;
	if (typeA -> level == Basic)
	{

	}
	else if (typeA -> level == Array)
	{

	}
	else if (typeA -> level == Structure)
	{

	}
	else
		printf("Fucking~\n");
}

