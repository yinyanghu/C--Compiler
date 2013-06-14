#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cmm.h"

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
	"Incompatible operands type", // 7
	"Incompatible return type", //8
	"Invalid arguments", // 9 <--------------
	"Invalid array", // 10
	"Invalid function", // 11
	"Operands type mistaken in array", // 12 <------------
	"Illegal use of '.'", // 13 <---------
	"Un-existed field", // 14
	"Redefined variable or initialize variable in struct", // 15
	"Redefined struct", // 16
	"Undefined struct", // 17
	"Function declared but undefined", // 18
	"Function inconsistent between declaration and defination", // 19
	"Not satisfied the assumptions!" // 20
};

int errorline;

void SemanticChecker(int lineno, int error_type)
{
	++ ErrorCounter;
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
struct Argument	*Build_Argument(char *name, struct TYPE *type, struct Argument *next)
{
	struct Argument	*ptr = (struct Argument *)malloc(sizeof(struct Argument));

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

struct VariableAttribute	*Build_VariableAttribute(struct TYPE *type, int vint, float vfloat, unsigned int address, int size, int no)
{
	struct VariableAttribute	*ptr = (struct VariableAttribute *)malloc(sizeof(struct VariableAttribute));
	ptr -> type = type;
	if (type == TYPE_INT)
		ptr -> v_int = vint;
	else
		ptr -> v_float = vfloat;
	ptr -> address = address;
	ptr -> size = size;
	ptr -> no = no;

	return ptr;
}

struct FunctionAttribute	*Build_FunctionAttribute(struct TYPE *type, struct Argument *args, unsigned int address, int size, FuncStatus status)
{
	struct FunctionAttribute	*ptr = (struct FunctionAttribute *)malloc(sizeof(struct FunctionAttribute));

	ptr -> return_type = type;
	ptr -> args = args;
	ptr -> address = address;
	ptr -> size = size;
	ptr -> status = status;

	return ptr;
}

struct StructureAttribute	*Build_StructureAttribute(struct StructureType *type, int size)
{
	struct StructureAttribute	*ptr = (struct StructureAttribute *)malloc(sizeof(struct StructureAttribute));

	ptr -> type = type;
	ptr -> size = size;

	return ptr;
}

struct Attribute	*Build_Attribute(AttributeType attr_type, struct FunctionAttribute *func, struct VariableAttribute *var, struct StructureAttribute *structure)
{
	struct Attribute	*ptr = (struct Attribute *)malloc(sizeof(struct Attribute));

	ptr -> type = attr_type;

	if (attr_type == Variable)
		ptr -> var = var;
	else if (attr_type == Function)
		ptr -> func = func;
	else if (attr_type == Struct)
		ptr -> structure = structure;
	else
		fprintf(stderr, "Fucking!\n");

	return ptr;
}

/* ================================================================================= */

void Structure_print(struct StructureType *ptr)
{
	fprintf(stderr, "{ ");
	for (; ptr != NULL; ptr = ptr -> next)
	{
		TYPE_print(ptr -> type);
		if (ptr -> next != NULL)
			fprintf(stderr, ", ");
	}
	fprintf(stderr, " }");
}

void TYPE_print(struct TYPE *ptr)
{
	if (ptr == NULL) return;
	if (ptr -> level == Basic)
	{
		if (ptr -> basic == TYPE_INT)
			fprintf(stderr, "int");
		else if (ptr -> basic == TYPE_FLOAT)
			fprintf(stderr, "float");
		else
			fprintf(stderr, "============FUCK=============");
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
		fprintf(stderr, "============FUCK=============");
}

void Argument_print(struct Argument *ptr)
{
	fprintf(stderr, "(");
	while (ptr)
	{
		TYPE_print(ptr -> type);
		if (ptr -> next != NULL)
			fprintf(stderr, ", ");
		ptr = ptr -> next;
	}
	fprintf(stderr, ")");
}

void Function_print(struct FunctionAttribute *ptr)
{
	fprintf(stderr, "Return Type: ");
	TYPE_print(ptr -> return_type);
	fprintf(stderr, "\n");

	fprintf(stderr, "Arguments: ");
	Argument_print(ptr -> args);
	fprintf(stderr, "\n");
}

void Attribute_print(struct Attribute *ptr)
{
	if (ptr -> type == Variable)
	{
		fprintf(stderr, "variable: ");
		TYPE_print(ptr -> var -> type);
		fprintf(stderr, "\n");
	}
	else if (ptr -> type == Function)
	{
		fprintf(stderr, "function: ");
		fprintf(stderr, "\n");
		Function_print(ptr -> func);
		fprintf(stderr, "\n");
	}
	else
		fprintf(stderr, "============FUCK=============");
}

void Array_print(struct ArrayType *ptr)
{
	TYPE_print(ptr -> next);
	fprintf(stderr, "[%d]", ptr -> size);
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
	DFL_clear();

	// For IR Code
	//IRST_clear(IRST);

	IR_Initialize();

	SemanticCheck_Program(AST);

	//IRST_test(IRST);

	/*
	fprintf(stderr, "=================================\n");
	DST_print(DST);
	fprintf(stderr, "*********************************\n");
	Scope_print(&Scope);
	fprintf(stderr, "=================================\n");
	*/
}


// YES
void SemanticCheck_Program(struct Program *root)
{
	errorline = TreeNode_GetLineno(&(root -> tree));

	Scope_push(&Scope);

	// Read(), Write()
	//struct IRSymbolsTable *IRread = IRST_insert(IRST, "read");
	struct SymbolsTable	*read = ST_insert(ST, "read", &Scope);
	struct Attribute *read_attr = Build_Attribute(Function, Build_FunctionAttribute(basic_int, NULL, 0, 0, Defined), NULL, NULL);
	//IRread -> attr = read -> attr = read_attr;
	read -> attr = read_attr;


	//struct IRSymbolsTable *IRwrite = IRST_insert(IRST, "write");
	struct SymbolsTable	*write = ST_insert(ST, "write", &Scope);
	struct Attribute *write_attr = Build_Attribute(Function, Build_FunctionAttribute(basic_int, Build_Argument("", basic_int, NULL), 0, 0, Defined), NULL, NULL);
	//IRwrite -> attr = write -> attr = write_attr;
	write -> attr = write_attr;

	SemanticCheck_ExtDefList(root -> extdeflist);

	// Here: Scope_top = 1

	Checking_DFL();
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
		//need report Error? I don't think so.
		return;
	}
	SemanticCheck_ExtDecList(ptr -> extdeclist, type);
}

// YES
void SemanticCheck_ExtDef_B(void *root)
{
	struct ExtDef_B		*ptr = (struct ExtDef_B *)root;
	SemanticCheck_Specifier(ptr -> specifier);
}

// YES
void SemanticCheck_ExtDef_C(void *root)
{
	struct ExtDef_C		*ptr = (struct ExtDef_C *)root;

	struct TYPE		*return_type = SemanticCheck_Specifier(ptr -> specifier);
	if (return_type == NULL)
	{
		//need report Error? I don't think so.
		return;
	}

	if (SemanticCheck_FunDec(ptr -> fundec, return_type, Defined) == 0)
	{
		ptr -> fundec -> code = IR_FunDec(ptr -> fundec);
		SemanticCheck_CompSt(ptr -> compst, return_type);
	}

	// IR Code
	ptr -> code = IR_ExtDef_C(root);
	
	Scope_pop(&Scope);
	DST_Global();
}

// YES
void SemanticCheck_ExtDef_D(void *root)
{
	struct ExtDef_D		*ptr = (struct ExtDef_D *)root;

	struct TYPE		*return_type = SemanticCheck_Specifier(ptr -> specifier);
	if (return_type == NULL)
	{
		//need report Error? I don't think so.
		return;
	}

	SemanticCheck_FunDec(ptr -> fundec, return_type, Declared);
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
			if (DST_check_name(DST, name) != 0)
			{
				if (ST_find_scope(ST, name, &Scope) == NULL)
				{
					/*
					if (DST_check_structure(DST, struct_type) == 0)
					{
						SemanticChecker(saved_errorline, StructMultiDefined);
						fprintf(stderr, "\tStruct '%s' \n", name);
					}
					else
					*/
					DST = DST_insert(DST, name, struct_type);
					
					// For IR Code
					//struct IRSymbolsTable *IRnew = IRST_insert(IRST, name);
					//IRnew -> attr = Build_Attribute(Struct, NULL, NULL, Build_StructureAttribute(struct_type, 0));
					
				}
				else
				{
					SemanticChecker(saved_errorline, StructMultiDefined);
					fprintf(stderr, "\tStruct name '%s' used in the previous defined variable/function\n", name);
				}
			}
			else
			{
				SemanticChecker(saved_errorline, StructMultiDefined);
				fprintf(stderr, "\tName '%s' used in the previous defined struct\n", name);
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
		SemanticChecker(errorline, StructUndefined);
		fprintf(stderr, "\tStruct '%s' is previously undefined\n", name);
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

struct Argument	*SemanticCheck_Argument_VarDec(struct VarDec *root, struct TYPE *type)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	return root -> SemanticArgumentCheck(root -> next, type);
}

// YES
struct SymbolsTable *SemanticCheck_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(ST, ptr -> id -> name, &Scope) != NULL)
	{
		SemanticChecker(errorline, VarMultiDefined);
		fprintf(stderr, "\tVariable '%s' is previously defined / Variable name '%s' conflicts function '%s' previously defined\n", ptr -> id -> name, ptr -> id -> name, ptr -> id -> name);
		return;
	}
	if (DST_check_name(DST, ptr -> id -> name) == 0)
	{
		SemanticChecker(errorline, VarMultiDefined);
		fprintf(stderr, "\tVariable name '%s' conflicts 'struct %s' previously defined\n", ptr -> id -> name, ptr -> id -> name);
		return;
	}
	struct SymbolsTable *new = ST_insert(ST, ptr -> id -> name, &Scope);
	new -> attr = Build_Attribute(Variable, NULL, Build_VariableAttribute(type, 0, 0, 0, 0, -1), NULL);	// <-----

	// For IR Code
	//struct IRSymbolsTable *IRnew = IRST_insert(IRST, ptr -> id -> name);
	//IRnew -> attr = new -> attr;		// <---- Bug?

	return new;
}

// YES
struct StructureType *SemanticCheck_Structure_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(SST, ptr -> id -> name, &StructScope) != NULL)
	{
		SemanticChecker(errorline, StructDefineError);
		fprintf(stderr, "\tVariable '%s' is previously defined in the struct\n", ptr -> id -> name);
		return NULL;
	}

	struct SymbolsTable *new = ST_insert(SST, ptr -> id -> name, &StructScope);
	new -> attr = Build_Attribute(Variable, NULL, Build_VariableAttribute(type, 0, 0, 0, 0, -1), NULL);	// <-----

	return Build_Structure(ptr -> id -> name, type, NULL);
}

// YES
struct Argument	*SemanticCheck_Argument_VarDec_A(void *root, struct TYPE *type)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	if (ST_find_scope(ST, ptr -> id -> name, &Scope) != NULL)
	{
		SemanticChecker(errorline, FuncArgumentMismatch);
		fprintf(stderr, "\tVariable '%s' is previously defined in the paramters of the function\n", ptr -> id -> name);
		return NULL;
	}

	struct SymbolsTable * new = ST_insert(ST, ptr -> id -> name, &Scope);
	new -> attr = Build_Attribute(Variable, NULL, Build_VariableAttribute(type, 0, 0, 0, 0, -1), NULL);	// <-----

	// For IR Code
	//struct IRSymbolsTable *IRnew = IRST_insert(IRST, ptr -> id -> name);
	//IRnew -> attr = new -> attr;		// <---- Bug?

	return Build_Argument(ptr -> id -> name, type, NULL);
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
struct Argument	*SemanticCheck_Argument_VarDec_B(void *root, struct TYPE *type)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;

	struct TYPE		*new = (struct TYPE *)malloc(sizeof(struct TYPE));
	new -> level = Array;
	new -> array = Build_Array(type, ptr -> exp_int -> key);

	return SemanticCheck_Argument_VarDec(ptr -> vardec, new);
	// Should Delete Type if failed
}

// YES
int SemanticCheck_FunDec(struct FunDec *root, struct TYPE *return_type, FuncStatus status)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	struct SymbolsTable		*finding = ST_find_scope(ST, root -> id -> name, &Scope);
	if (finding != NULL)
	{
		Scope_push(&Scope); // Scope_pop in ExtDef_C/D
		DST_Local();

		if (finding -> attr -> type != Function)
		{
			SemanticChecker(errorline, UnexpectedError);
			fprintf(stderr, "\t'%s' redeclared as different kind of symbol\n", root -> id -> name);
			return -1;
		}
		else if (finding -> attr -> type != Variable)
		{
			struct FunctionAttribute	*attr = finding -> attr -> func;
			if (attr -> status == Defined && status == Defined)
			{
				SemanticChecker(errorline, FuncMultiDefined);
				fprintf(stderr, "\tFunction '%s' is previously defined\n", root -> id -> name);
				return -1;
			}
			else
			{
				struct Argument *args = SemanticCheck_VarList(root -> varlist);
				if (Checking_Function(attr, Build_FunctionAttribute(return_type, args, 0, 0, status)) != 0)
				{
					if (status == Defined)
					{
						attr -> status = Defined;
						DFL_remove(&DFL, root -> id -> name);
					}
					return 0;
				}
				errorline = TreeNode_GetLineno(&(root -> tree));
				SemanticChecker(errorline, FuncDefineConflict);
				fprintf(stderr, "\tConflicting type for function '%s'\n", root -> id -> name);
				return -1;
			}
		}
		else
			fprintf(stderr, "Fucking!\n");
	}

	struct SymbolsTable *func = ST_insert(ST, root -> id -> name, &Scope);

	Scope_push(&Scope); // Scope_pop in ExtDef_C
	DST_Local();

	struct Argument *args = SemanticCheck_VarList(root -> varlist);

	func -> attr = Build_Attribute(Function, Build_FunctionAttribute(return_type, args, 0, 0, status), NULL, NULL); // <------

	// For IR Code
	//struct IRSymbolsTable *IRnew = IRST_insert(IRST, root -> id -> name);
	//IRnew -> attr = func -> attr;		// <---- Bug?
	
	if (status == Declared)
	{
		errorline = TreeNode_GetLineno(&(root -> tree));
		DFL = DFL_insert(DFL, root -> id -> name, errorline);
	}
	return 0;
}

// YES?
struct Argument *SemanticCheck_VarList(struct VarList *root)
{
	if (root == NULL) return NULL;

	struct Argument	*ptr = SemanticCheck_ParamDec(root -> paramdec);
	if (ptr == NULL) return SemanticCheck_VarList(root -> varlist);
	ptr -> next = SemanticCheck_VarList(root -> varlist);
	return ptr;
}

// YES?
struct Argument *SemanticCheck_ParamDec(struct ParamDec *root)
{
	struct TYPE		*type = SemanticCheck_Specifier(root -> specifier);
	if (type == NULL) return NULL;
	return SemanticCheck_Argument_VarDec(root -> vardec, type);
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
		SemanticChecker(errorline, ReturnMismatch);
		fprintf(stderr, "\tExpected return type '");
		TYPE_print(return_type);	
		fprintf(stderr, "'\n");
	}
}


// YES
void SemanticCheck_Stmt_CompSt(void *root, struct TYPE *return_type)
{
	struct Stmt_CompSt	*ptr = (struct Stmt_CompSt *)root;
	Scope_push(&Scope);
	SemanticCheck_CompSt(ptr -> compst, return_type);

	// IR Code
	ptr -> code = IR_Stmt_CompSt(root);

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
	{
		struct TYPE		*exp_type = SemanticCheck_Exp(root -> exp);
		if (exp_type == NULL) return;
		if (Checking_MatchingType(type, exp_type) == 0)
		{
			SemanticChecker(errorline, ExpMismatch);
			fprintf(stderr, "\tExpected type '");
			TYPE_print(type);
			fprintf(stderr, "'\n");
			//return;
		}
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
		SemanticChecker(errorline, StructDefineError);
		fprintf(stderr, "\tCannot initialize the variable in struct\n");
		return NULL;
	}

	return SemanticCheck_Structure_VarDec(root -> vardec, type);
}

// YES
struct TYPE *SemanticCheck_Exp(struct Exp *root)
{
	errorline = TreeNode_GetLineno(&(root -> tree));
	root -> exp_type = root -> SemanticCheck(root -> next);
	return (root -> exp_type);
}


// YES
struct TYPE *SemanticCheck_Exp_Assign(void *root)
{
	struct Exp_Assign		*ptr = (struct Exp_Assign *)root;
	struct TYPE		*L = SemanticCheck_Exp(ptr -> exp_left), *R = SemanticCheck_Exp(ptr -> exp_right);

	if (L == NULL || R == NULL) return NULL;

	if (ptr -> exp_left -> left_value == 0)
	{
		SemanticChecker(errorline, LeftValueError);
		fprintf(stderr, "\tExpected a L-value as left operand of assignment\n");
		return NULL;
	}

	if (Checking_MatchingType(L, R) == 0)
	{
		SemanticChecker(errorline, ExpMismatch);
		fprintf(stderr, "\tExpected type '");
		TYPE_print(L);
		fprintf(stderr, "'\n");
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
		SemanticChecker(errorline, UnexpectedError);
		return NULL;
	}
	if (R -> level != Basic || R -> basic != TYPE_INT)
	{
		SemanticChecker(errorline, UnexpectedError);
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
			SemanticChecker(errorline, UnexpectedError);
			return NULL;
		}
		if (R -> level != Basic || R -> basic != TYPE_INT)
		{
			SemanticChecker(errorline, UnexpectedError);
			return NULL;
		}
	}
	else
	{
		if (Checking_MatchingType(L, R) == 0)
		{
			SemanticChecker(errorline, OpTypeMismatch);
			fprintf(stderr, "\tInvalid operands to binary %s (have '", Sign_BinaryOP(ptr -> op));
			TYPE_print(L);
			fprintf(stderr, "' and '");
			TYPE_print(R);
			fprintf(stderr, "')\n");
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
				SemanticChecker(errorline, OpTypeMismatch);
				fprintf(stderr, "\tInvalid operands to minus '-' (have '");
				TYPE_print(type);
				fprintf(stderr, "')\n");
				return NULL;
			}
		}
		else
		{
			if (type -> level != Basic || type -> basic != TYPE_INT)
			{
				SemanticChecker(errorline, UnexpectedError);
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
		SemanticChecker(errorline, FuncUndefined);
		fprintf(stderr, "\tFunction '%s' is previously undefined\n", ptr -> func -> name);
		return NULL;
	}

	assert(!(finding -> attr == NULL));

	if (finding -> attr -> type != Function)
	{
		SemanticChecker(errorline, FuncCallOpError);
		fprintf(stderr, "\t'%s' is NOT a function\n", ptr -> func -> name);
		return NULL;
	}

	struct FunctionAttribute	*attr = finding -> attr -> func;
	if (!Checking_Argument_Args(ptr -> args, attr -> args))
	{
		SemanticChecker(errorline, FuncArgumentMismatch);
		fprintf(stderr, "\tIncompatible arguments to function '%s', expected type '", ptr -> func -> name);
		Argument_print(attr -> args);
		fprintf(stderr, "'\n");
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
		SemanticChecker(errorline, ArrayAccessError);
		fprintf(stderr, "\tArray subscript is NOT an integer\n");
		return NULL;
	}

	struct TYPE		*inner = SemanticCheck_Exp(ptr -> array);
	if (inner == NULL) return NULL;

	if (inner -> level != Array)
	{
		SemanticChecker(errorline, ArrayOpError);
		fprintf(stderr, "\tIt is NOT an array\n");
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
		SemanticChecker(errorline, StructAttributeError);
		fprintf(stderr, "\tRequest for member '%s' in something NOT a structure\n", ptr -> attribute -> name);
		return NULL;
	}

	struct StructureType	*entry;
	for (entry = head -> structure; entry != NULL; entry = entry -> next)
		if (strcmp(entry -> name, ptr -> attribute -> name) == 0)		
			return (entry -> type);

	SemanticChecker(errorline, AccessStructUndefinedField);
	fprintf(stderr, "\tStruct has no member named '%s'\n", ptr -> attribute -> name);
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
		SemanticChecker(errorline, VarUndefined);
		fprintf(stderr, "\tVariable '%s' is previously undefined\n", ptr -> var -> name);
		return NULL;
	}
	assert(!(finding -> attr == NULL));

	if (finding -> attr -> type != Variable)
	{
		// Not a variable
		SemanticChecker(errorline, VarUndefined);
		fprintf(stderr, "\t'%s' is NOT a variable\n", ptr -> var -> name);
		return NULL;
	}
	struct VariableAttribute	*attr = finding -> attr -> var;
	return (attr -> type);
	
}

/* ================================================================================= */

void Checking_DFL(void)
{
	if (DFL_check(DFL) != 0) return;
	struct DeclaredFunctionList		*ptr;
	for (ptr = DFL; ptr != NULL; ptr = ptr -> next)
	{
		SemanticChecker(ptr -> lineno, FuncDeclareButUndefined);
		fprintf(stderr, "\tFunction '%s' is declared but undefined\n", ptr -> name);
	}

}

int Checking_Argument_Args(struct Args *root, struct Argument *args)
{
	for (; root != NULL && args != NULL; root = root -> args, args = args -> next)
		if (Checking_MatchingType(SemanticCheck_Exp(root -> exp), args -> type) == 0) return 0;
	return (root == NULL && args == NULL ? 1 : 0);
}

int Checking_Argument(struct Argument *argA, struct Argument *argB)
{
	for (; argA != NULL && argB != NULL; argA = argA -> next, argB = argB -> next)
		if (Checking_MatchingType(argA -> type, argB -> type) == 0) return 0;
	return (argA == NULL && argB == NULL ? 1 : 0);
}

int Checking_Function(struct FunctionAttribute *funcA, struct FunctionAttribute *funcB)
{
	if (Checking_MatchingType(funcA -> return_type, funcB -> return_type) == 0) return 0;		
	return Checking_Argument(funcA -> args, funcB -> args);
}

void Checking_Condition(struct Exp *exp)
{
	errorline = TreeNode_GetLineno(&(exp -> tree));
	struct TYPE		*type = SemanticCheck_Exp(exp);
	if (type == NULL) return;
	if (Checking_MatchingType(type, basic_int) == 0)
	{
		SemanticChecker(errorline, UnexpectedError);
	}
}


int Checking_MatchingType(struct TYPE *typeA, struct TYPE *typeB)		// 1 --> Match, 0 --> Mismatch
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
		fprintf(stderr, "Fucking~\n");
}

/* ================================================================================= */

void Remove_Structure(struct StructureType *ptr)
{
	if (ptr == NULL) return;
	Remove_Structure(ptr -> next);
	free(ptr);
}


