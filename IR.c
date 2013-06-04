#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmm.h"

extern	int ErrorCounter;

int		LabelCounter;
int		VarCounter;
int		TempCounter;

int		Lvalue;

struct IRChain		*IR;

struct Operand		*Zero, *One;

#define MaxIndex	30

/*
struct IndexType
{
	enum {
		Offset_Attribute,
		Index_Array
	} type;
	struct Operand	*key;
} ArrayIndex[MaxIndex];
*/
struct Operand	*ArrayIndex[MaxIndex];

int IndexPtr;

int Attribute_Offset;

int isbasic(struct TYPE *ptr)
{
	return ((ptr -> level == Basic) ? 1 : -1);
}

int typesize(struct TYPE *ptr)
{
	if (ptr -> level == Basic)
		return 4;
	else if (ptr -> level == Array)
		return ((ptr -> array -> size) * typesize(ptr -> array -> next));
	else if (ptr -> level == Structure)
	{
		int ret = 0;
		struct StructureType *x = ptr -> structure;	
		while (x)
		{
			ret += typesize(x -> type);
			x = x -> next;	
		}
		return ret;
	}
}

int offset(struct StructureType *ptr, char *name)
{
	int ret = 0;
	while (ptr)
	{
		if (strcmp(name, ptr -> name) == 0) 
			return ret;
		ret += typesize(ptr -> type);
		ptr = ptr -> next;
	}
	fprintf(stderr, "Fuck 009!\n");
}

int	get_label(void)
{
	++ LabelCounter;
	return LabelCounter;
}

int get_temp(void)
{
	++ TempCounter;
	return TempCounter;
}

int get_var(struct VariableAttribute *attr)
{
	/*
	if (attr -> type -> level != Basic)
		return -1;
	*/

	if (attr -> no == -1)
	{
		++ VarCounter;
		attr -> no = VarCounter;
	}
	return attr -> no;
}


struct Operand	*ID_Operand(char *name, int is_ptr)
{
	struct Attribute	*attr = IRST_find(IRST, name) -> attr;

	if (attr -> type != Variable)
	{
		fprintf(stderr, "Fuck 001\n");
		return NULL;
	}

	int no = get_var(attr -> var);

	attr -> var -> is_ptr = is_ptr;

	return Build_Operand(VARIABLE, no);
}

struct Operand	*Build_Operand(OperandType type, int key)
{
	struct Operand		*ptr = (struct Operand *)malloc(sizeof(struct Operand));
	ptr -> type = type;
	if (type == VARIABLE)
		ptr -> var_no = key;
	else if (type == TEMP)
		ptr -> temp_no = key;
	else if (type == CONSTANT)
		ptr -> constant = key;
	else if (type == ADDRESS)
		ptr -> addr = (unsigned int)key;	
	else
		fprintf(stderr, "Fuck 003\n");
	// WTF
	return ptr;
}

struct IRCode	*Build_IRCode(IRCodeType type, struct Operand *A, struct Operand *B, struct Operand *C, BinaryOP_Relop op, int key, char *func)
{
	struct IRCode		*ptr = (struct IRCode *)malloc(sizeof(struct IRCode));

	ptr -> type = type;

	if (type == LABEL || type == GOTO)
	{
		ptr -> label = key;
	}
	else if (type == FUNCTION)
	{
		strcpy(ptr -> func, func);	
	}
	else if (type == ARG || type == PARAM || type == READ || type == WRITE || type == RETURNRETURN)
	{
		ptr -> single = A;
	}
	else if (type == CALL)
	{
		strcpy(ptr -> call.func, func);
		ptr -> call.ret = A;
	}
	else if (type == ASSIGN || type == ValueAddress || type == AddressValueL || type == AddressValueR)
	{
		ptr -> lr.left = A;
		ptr -> lr.right = B;
	}
	else if (type == ADD || type == SUB || type == MUL || type == DIVDIV)
	{
		ptr -> binary.result = A;
		ptr -> binary.op1 = B;
		ptr -> binary.op2 = C;

	}
	else if (type == CONDGOTO)
	{
		ptr -> condjump.op = op;
		ptr -> condjump.left = A;
		ptr -> condjump.right = B;
		ptr -> condjump.next = key;
	}
	else if (type == DEC)
	{
		ptr -> dec.x = A;
		ptr -> dec.size = key;
	}

	return ptr;
}


void Print_Operand(struct Operand *ptr)
{
	if (ptr -> type == VARIABLE)
		fprintf(stdout, "v%d", ptr -> var_no);
	else if (ptr -> type == TEMP)
		fprintf(stdout, "t%d", ptr -> temp_no);
	else if (ptr -> type == CONSTANT)
		fprintf(stdout, "#%d", ptr -> constant);
	else
		fprintf(stdout, "%u", ptr -> addr);
}

char *Print_RELOP(BinaryOP_Relop op)
{
	if (op == OP_EQ)
		return "==";
	else if (op == OP_NE)
		return "!=";
	else if (op == OP_LT)
		return "<";
	else if (op == OP_GT)
		return ">";
	else if (op == OP_GE)
		return ">=";
	else if (op == OP_LE)
		return "<=";
	else
		return "Fuck 007";
}

void Print_IRCode(struct IRCode *ptr)
{
	IRCodeType type = ptr -> type;

	if (type == LABEL)
		fprintf(stdout, "LABEL L%d :", ptr -> label);
	else if (type == FUNCTION)
		fprintf(stdout, "FUNCTION %s :", ptr -> func);
	else if (type == GOTO)
		fprintf(stdout, "GOTO L%d", ptr -> label);
	else if (type == ARG)
	{
		fprintf(stdout, "ARG ");
		Print_Operand(ptr -> single);
	}
	else if (type == PARAM)
	{
		fprintf(stdout, "PARAM ");
		Print_Operand(ptr -> single);
	}
	else if (type == RETURNRETURN)
	{
		fprintf(stdout, "RETURN ");
		Print_Operand(ptr -> single);
	}
	else if (type == READ)
	{
		fprintf(stdout, "READ ");
		Print_Operand(ptr -> single);
	}
	else if (type == WRITE)
	{
		fprintf(stdout, "WRITE ");
		Print_Operand(ptr -> single);
	}
	else if (type == ASSIGN)
	{
		Print_Operand(ptr -> lr.left);
		fprintf(stdout, " := ");
		Print_Operand(ptr -> lr.right);
	}
	else if (type == CALL)
	{
		Print_Operand(ptr -> call.ret);
		fprintf(stdout, " := CALL %s", ptr -> call.func);
	}
	else if (type == ValueAddress)
	{
		Print_Operand(ptr -> lr.left);
		fprintf(stdout, " := &");
		Print_Operand(ptr -> lr.right);
	}
	else if (type == AddressValueL)
	{
		fprintf(stdout, "*");
		Print_Operand(ptr -> lr.left);
		fprintf(stdout, " := ");
		Print_Operand(ptr -> lr.right);
	}
	else if (type == AddressValueR)
	{
		Print_Operand(ptr -> lr.left);
		fprintf(stdout, " := *");
		Print_Operand(ptr -> lr.right);
	}
	else if (type == ADD)
	{
		Print_Operand(ptr -> binary.result);	
		fprintf(stdout, " := ");
		Print_Operand(ptr -> binary.op1);	
		fprintf(stdout, " + ");
		Print_Operand(ptr -> binary.op2);	
	}
	else if (type == SUB)
	{
		Print_Operand(ptr -> binary.result);	
		fprintf(stdout, " := ");
		Print_Operand(ptr -> binary.op1);	
		fprintf(stdout, " - ");
		Print_Operand(ptr -> binary.op2);	
	}
	else if (type == MUL)
	{
		Print_Operand(ptr -> binary.result);	
		fprintf(stdout, " := ");
		Print_Operand(ptr -> binary.op1);	
		fprintf(stdout, " * ");
		Print_Operand(ptr -> binary.op2);	
	}
	else if (type == DIVDIV)
	{
		Print_Operand(ptr -> binary.result);	
		fprintf(stdout, " := ");
		Print_Operand(ptr -> binary.op1);	
		fprintf(stdout, " / ");
		Print_Operand(ptr -> binary.op2);	
	}
	else if (type == CONDGOTO)
	{
		fprintf(stdout, "IF ");
		Print_Operand(ptr -> condjump.left);
		fprintf(stdout, " %s ", Print_RELOP(ptr -> condjump.op));
		Print_Operand(ptr -> condjump.right);
		fprintf(stdout, " GOTO L%d", ptr -> condjump.next);
	}
	else if (type == DEC)
	{
		fprintf(stdout, "DEC ");
		Print_Operand(ptr -> dec.x);
		fprintf(stdout, " %d", ptr -> dec.size);
	}

	fprintf(stdout, "\n");
}
/*
struct IRChain	*IRChain_clear(void)
{
	struct IRChain	*ptr = (struct IRChain *)malloc(sizeof(struct IRChain));
	ptr -> prev = ptr -> next = ptr;
	ptr -> code = NULL;

	return ptr;
}
*/

struct IRChain *IRChain_build(struct IRCode *code)
{
	struct IRChain	*new = (struct IRChain *)malloc(sizeof(struct IRChain));
	new -> code = code;
	new -> prev = new -> next = new;

	return new;
}

struct IRChain *IRChain_link(struct IRChain *A, struct IRChain *B)
{
	if (A == NULL && B == NULL) return NULL;
	if (A == NULL) return B;
	if (B == NULL) return A;

	A -> prev -> next = B;
	B -> prev -> next = A;

	struct IRChain *temp = A -> prev;
	A -> prev = B -> prev;
	B -> prev = temp;

	return A;
}

void Print_IRChain(struct IRChain *header)
{
	struct IRChain	*ptr = header;
	while (1)
	{
		Print_IRCode(ptr -> code);
		ptr = ptr -> next;
		if (ptr == header) break;
	}
	// UGLY @@@
}

//===============================================================

void GeneratingIR(struct Program *AST)
{
	// Clear Counters
	TempCounter = LabelCounter = VarCounter = 0;
	IndexPtr = 0;
	Attribute_Offset = 0;
	Lvalue = 0;

	// Constant #0, #1
	Zero = Build_Operand(CONSTANT, 0);
	One = Build_Operand(CONSTANT, 1);

	IR = IR_Program(AST);

	Print_IRChain(IR);
}

struct IRChain *IR_Program(struct Program *root)
{
	return IR_ExtDefList(root -> extdeflist);	
}

struct IRChain *IR_ExtDefList(struct ExtDefList *root)
{
	struct IRChain	*ret = NULL;
	while (root)
	{
		ret = IRChain_link(ret, IR_ExtDef(root -> extdef));
		root = root -> extdeflist;
	}
	return ret;
}

struct IRChain *IR_ExtDef(struct ExtDef *root)
{
	return (root -> IR(root -> next));	
}

struct IRChain *IR_ExtDef_A(void *root)
{
	fprintf(stderr, "You are a Liar!\n");
	return NULL;
}

struct IRChain *IR_ExtDef_B(void *root)
{
	return NULL;
}

struct IRChain *IR_ExtDef_C(void *root)
{
	struct ExtDef_C		*ptr = (struct ExtDef_C *)root;

	struct IRChain	*fundec = IR_FunDec(ptr -> fundec);
	struct IRChain	*compst = IR_CompSt(ptr -> compst);

	return IRChain_link(fundec, compst);
}

struct IRChain *IR_ExtDef_D(void *root)
{
	fprintf(stderr, "You are a Liar!\n");
	return NULL;
}

struct IRChain *IR_FunDec(struct FunDec *root)
{
	struct Attribute	*attr = IRST_find(IRST, root -> id -> name) -> attr;

	struct IRCode	*code = Build_IRCode(FUNCTION, NULL, NULL, NULL, 0, 0, root -> id -> name);

	struct IRChain	*ret = IRChain_build(code);

	if (attr -> type != Function)
	{
		fprintf(stderr, "Fuck 008\n");
	}
	
	struct Argument	*args = attr -> func -> args;
	while (args)
	{
		// @@@
		int is_ptr = (args -> type -> level == Basic) ? 0 : 1;
		struct Operand	*P = ID_Operand(args -> name, is_ptr);
		//fprintf(stderr, "is_ptr = %d\n", is_ptr);
		struct IRChain	*Pcode = IRChain_build(Build_IRCode(PARAM, P, NULL, NULL, 0, 0, NULL));
		ret = IRChain_link(ret, Pcode);
		args = args -> next;
	}

	return ret;
}

struct IRChain *IR_CompSt(struct CompSt *root)
{
	struct IRChain	*deflist = IR_DefList(root -> deflist);
	struct IRChain	*stmtlist = IR_StmtList(root -> stmtlist);

	return IRChain_link(deflist, stmtlist);
}

struct IRChain *IR_DefList(struct DefList *root)
{
	struct IRChain	*ret = NULL;
	while (root)
	{
		ret = IRChain_link(ret, IR_Def(root -> def));
		root = root -> deflist;
	}
	return ret;
}

struct IRChain *IR_StmtList(struct StmtList *root)
{
	struct IRChain	*ret = NULL;
	while (root)
	{
		ret = IRChain_link(ret, IR_Stmt(root -> stmt));
		root = root -> stmtlist;
	}
	return ret;
}

struct IRChain *IR_Stmt(struct Stmt *root)
{
	return (root -> IR(root -> next));
}

struct IRChain *IR_Stmt_Exp(void *root)
{
	struct Stmt_Exp		*ptr = (struct Stmt_Exp *)root;

	return IR_Exp(ptr -> exp, NULL);
}

struct IRChain *IR_Stmt_Return(void *root)
{
	struct Stmt_Return	*ptr = (struct Stmt_Return *)root;

	struct Operand	*t1 = Build_Operand(TEMP, get_temp());

	struct IRChain	*codeA = IR_Exp(ptr -> exp, t1);
	struct IRChain	*codeB = IRChain_build(Build_IRCode(RETURNRETURN, t1, NULL, NULL, 0, 0, NULL));

	return IRChain_link(codeA, codeB);
}

struct IRChain *IR_Stmt_CompSt(void *root)
{
	struct Stmt_CompSt	*ptr = (struct Stmt_CompSt *)root;

	return IR_CompSt(ptr -> compst);
	// Bug?
}

struct IRChain *IR_Stmt_If(void *root)
{
	struct Stmt_If		*ptr = (struct Stmt_If *)root;

	int l1 = get_label();
	int l2 = get_label();

	struct IRChain	*codeA = IR_Condition(ptr -> exp, l1, l2);
	struct IRChain	*codeB = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l1, NULL));
	struct IRChain	*codeC = IR_Stmt(ptr -> stmt);
	struct IRChain	*codeD = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l2, NULL));

	codeA = IRChain_link(codeA, codeB);
	codeA = IRChain_link(codeA, codeC);
	codeA = IRChain_link(codeA, codeD);

	return codeA;
}

struct IRChain *IR_Stmt_If_Else(void *root)
{
	struct Stmt_If_Else		*ptr = (struct Stmt_If_Else *)root;

	int l1 = get_label();
	int l2 = get_label();
	int l3 = get_label();

	struct IRChain	*codeA = IR_Condition(ptr -> exp, l1, l2);
	struct IRChain	*codeB = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l1, NULL));
	struct IRChain	*codeC = IR_Stmt(ptr -> stmt_if);
	struct IRChain	*codeD = IRChain_build(Build_IRCode(GOTO, NULL, NULL, NULL, 0, l3, NULL));
	struct IRChain	*codeE = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l2, NULL));
	struct IRChain	*codeF = IR_Stmt(ptr -> stmt_else);
	struct IRChain	*codeG = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l3, NULL));

	codeA = IRChain_link(codeA, codeB);
	codeA = IRChain_link(codeA, codeC);
	codeA = IRChain_link(codeA, codeD);
	codeA = IRChain_link(codeA, codeE);
	codeA = IRChain_link(codeA, codeF);
	codeA = IRChain_link(codeA, codeG);

	return codeA;
}

struct IRChain *IR_Stmt_While(void *root)
{
	struct Stmt_While		*ptr = (struct Stmt_While *)root;

	int l1 = get_label();
	int l2 = get_label();
	int l3 = get_label();

	struct IRChain	*codeA = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l1, NULL));
	struct IRChain	*codeB = IR_Condition(ptr -> exp, l2, l3);
	struct IRChain	*codeC = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l2, NULL));
	struct IRChain	*codeD = IR_Stmt(ptr -> stmt);
	struct IRChain	*codeE = IRChain_build(Build_IRCode(GOTO, NULL, NULL, NULL, 0, l1, NULL));
	struct IRChain	*codeF = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l3, NULL));

	codeA = IRChain_link(codeA, codeB);
	codeA = IRChain_link(codeA, codeC);
	codeA = IRChain_link(codeA, codeD);
	codeA = IRChain_link(codeA, codeE);
	codeA = IRChain_link(codeA, codeF);

	return codeA;
}


struct IRChain *IR_Condition(struct Exp *root, int label_true, int label_false)
{
	if (root -> IR_Condition != NULL)
		return (root -> IR_Condition(root -> next, label_true, label_false));
	else
	{
		struct Operand	*t1 = Build_Operand(TEMP, get_temp());
		struct IRChain	*codeA = IR_Exp(root, t1);
		struct IRChain	*codeB = IRChain_build(Build_IRCode(CONDGOTO, t1, Zero, NULL, OP_NE, label_true, NULL));
		struct IRChain	*codeC = IRChain_build(Build_IRCode(GOTO, NULL, NULL, NULL, 0, label_false, NULL));

		codeA = IRChain_link(codeA, codeB);
		codeA = IRChain_link(codeA, codeC);

		return codeA;
	}
}


struct IRChain *IR_Condition_RELOP(void *root, int label_true, int label_false)
{
	struct Exp_Binary_Rel	*ptr = (struct Exp_Binary_Rel *)root;

	struct Operand	*t1 = Build_Operand(TEMP, get_temp());
	struct Operand	*t2 = Build_Operand(TEMP, get_temp());

	struct IRChain	*codeA = IR_Exp(ptr -> exp_a, t1);
	struct IRChain	*codeB = IR_Exp(ptr -> exp_b, t2);
	struct IRChain	*codeC = IRChain_build(Build_IRCode(CONDGOTO, t1, t2, NULL, ptr -> op, label_true, NULL));
	struct IRChain	*codeD = IRChain_build(Build_IRCode(GOTO, NULL, NULL, NULL, 0, label_false, NULL));

	codeA = IRChain_link(codeA, codeB);
	codeA = IRChain_link(codeA, codeC);
	codeA = IRChain_link(codeA, codeD);

	return codeA;
}

struct IRChain *IR_Condition_NOT(void *root, int label_true, int label_false)
{
	struct Exp_Unary	*ptr = (struct Exp_Unary *)root;

	return IR_Condition(ptr -> exp, label_false, label_true);
}

struct IRChain *IR_Condition_Par(void *root, int label_true, int label_false)
{
	struct Exp_Unary	*ptr = (struct Exp_Unary *)root;

	return IR_Condition(ptr -> exp, label_true, label_false);
}

struct IRChain *IR_Condition_AND(void *root, int label_true, int label_false)
{
	struct Exp_Binary_Cal	*ptr = (struct Exp_Binary_Cal *)root;

	int l1 = get_label();

	struct IRChain	*codeA = IR_Condition(ptr -> exp_a, l1, label_false);
	struct IRChain	*codeB = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l1, NULL));
	struct IRChain	*codeC = IR_Condition(ptr -> exp_b, label_true, label_false);

	codeA = IRChain_link(codeA, codeB);
	codeA = IRChain_link(codeA, codeC);

	return codeA;
}


struct IRChain *IR_Condition_OR(void *root, int label_true, int label_false)
{
	struct Exp_Binary_Cal	*ptr = (struct Exp_Binary_Cal *)root;

	int l1 = get_label();

	struct IRChain	*codeA = IR_Condition(ptr -> exp_a, label_true, l1);
	struct IRChain	*codeB = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l1, NULL));
	struct IRChain	*codeC = IR_Condition(ptr -> exp_b, label_true, label_false);

	codeA = IRChain_link(codeA, codeB);
	codeA = IRChain_link(codeA, codeC);

	return codeA;
}

struct IRChain *IR_Def(struct Def *root)
{
	return IR_DecList(root -> declist);		
}

struct IRChain *IR_DecList(struct DecList *root)
{
	struct IRChain	*ret = NULL;
	while (root)
	{
		ret = IRChain_link(ret, IR_Dec(root -> dec));
		root = root -> declist;
	}
	return ret;
}

struct IRChain *IR_Dec(struct Dec *root)
{
	struct IRChain	*codeA = NULL;
	struct Operand	*t = NULL;
	if (root -> exp != NULL)
	{
		t = Build_Operand(TEMP, get_temp());
		codeA= IR_Exp(root -> exp, t);
	}
	struct IRChain	*ret = IR_VarDec(root -> vardec, t);
	ret = IRChain_link(codeA, ret);
	return ret;
}


struct IRChain *IR_VarDec(struct VarDec *root, struct Operand *value)
{
	return (root -> IR(root -> next, value));
}


struct IRChain *IR_VarDec_A(void *root, struct Operand *value)
{
	struct VarDec_A		*ptr = (struct VarDec_A *)root;
	struct Attribute	*attr = IRST_find(IRST, ptr -> id -> name) -> attr;

	if (attr -> type != Variable)
	{
		fprintf(stderr, "Fuck 005\n");
	}

	if (attr -> var -> no != -1)
	{
		fprintf(stderr, "Fuck 002\n");
	}

	int no = get_var(attr -> var);

	if (value != NULL)
	{
		if (isbasic(attr -> var -> type) == -1)
		{
			++ ErrorCounter;
			fprintf(stderr, "Can not translate the code: initialize a struct or array variable.\n");
			return NULL;
		}
		struct Operand	*new = Build_Operand(VARIABLE, no);
		return IRChain_build(Build_IRCode(ASSIGN, new, value, NULL, 0, 0, NULL));
	}
	else if (isbasic(attr -> var -> type) == -1)
	{
		// structure or array
		int size = typesize(attr -> var -> type);
		struct Operand	*new = Build_Operand(VARIABLE, no);
		return IRChain_build(Build_IRCode(DEC, new, NULL, NULL, 0, size, NULL));
	}
	return NULL;
}

struct IRChain *IR_VarDec_B(void *root, struct Operand *value)
{
	struct VarDec_B		*ptr = (struct VarDec_B *)root;
	return IR_VarDec(ptr -> vardec, value);
}


struct IRChain *IR_Exp(struct Exp *root, struct Operand *place)
{
	if (root -> IR != NULL)
		return (root -> IR(root -> next, place, root -> exp_type));
	else	// @@@@
	{
		if (place == NULL) return NULL;

		int l1 = get_label();
		int l2 = get_label();

		struct IRChain	*codeA = IRChain_build(Build_IRCode(ASSIGN, place, Zero, NULL, 0, 0, NULL));
		struct IRChain	*codeB = IR_Condition(root, l1, l2);
		struct IRChain	*codeC = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l1, NULL));
		struct IRChain	*codeD = IRChain_build(Build_IRCode(ASSIGN, place, One, NULL, 0, 0, NULL));
		struct IRChain	*codeE = IRChain_build(Build_IRCode(LABEL, NULL, NULL, NULL, 0, l2, NULL));

		codeA = IRChain_link(codeA, codeB);
		codeA = IRChain_link(codeA, codeC);
		codeA = IRChain_link(codeA, codeD);
		codeA = IRChain_link(codeA, codeE);

		return codeA;
	}
}

struct IRChain *IR_Exp_INT(void *root, struct Operand *place, struct TYPE *exp_type)
{
	if (place == NULL) return NULL;

	struct Exp_INT	*ptr = (struct Exp_INT *)root;

	struct Operand	*value = Build_Operand(CONSTANT, ptr -> key);
	struct IRChain	*code = IRChain_build(Build_IRCode(ASSIGN, place, value, NULL, 0, 0, NULL));

	return code;
}


struct IRChain *IR_Address(struct TYPE *x, struct Operand *base, struct Operand *ret_addr, int base_is_ptr)
{
	struct IRChain	*code = NULL;
	if (x -> level == Array)	// Array
	{
		// Base Address
		if (base_is_ptr == 1)
			code = IRChain_link(code, IRChain_build(Build_IRCode(ASSIGN, ret_addr, base, NULL, 0, 0, NULL)));
		else
			code = IRChain_link(code, IRChain_build(Build_IRCode(ValueAddress, ret_addr, base, NULL, 0, 0, NULL)));

		// Offset
		struct Operand	*offset = Build_Operand(TEMP, get_temp());
		code = IRChain_link(code, IRChain_build(Build_IRCode(ASSIGN, offset, Zero, NULL, 0, 0, NULL)));
		int K = 1, i = 0;
		while (i < IndexPtr && x -> level == Array)
		{
			code = IRChain_link(code, IRChain_build(Build_IRCode(MUL, ArrayIndex[i], ArrayIndex[i], Build_Operand(CONSTANT, 4 * K), 0, 0, NULL)));
			code = IRChain_link(code, IRChain_build(Build_IRCode(ADD, offset, ArrayIndex[i], offset, 0, 0, NULL)));
			K *= x -> array -> size;
			x = x -> array -> next;
			++ i;
		}
		if (i != IndexPtr)
		{
			fprintf(stderr, "Fuck 101\n");
		}

		// Base Address(Offset)
		code = IRChain_link(code, IRChain_build(Build_IRCode(ADD, ret_addr, ret_addr, offset, 0, 0, NULL)));

		return code;
	}
	else if (x -> level == Structure) // Structure
	{
		// Base Address
		if (base_is_ptr == 1)
			code = IRChain_link(code, IRChain_build(Build_IRCode(ASSIGN, ret_addr, base, NULL, 0, 0, NULL)));
		else
			code = IRChain_link(code, IRChain_build(Build_IRCode(ValueAddress, ret_addr, base, NULL, 0, 0, NULL)));

		// Offset
		struct Operand	*offset = Build_Operand(CONSTANT, Attribute_Offset);

		// Base Address(Offset)
		code = IRChain_link(code, IRChain_build(Build_IRCode(ADD, ret_addr, ret_addr, offset, 0, 0, NULL)));

		return code;
	}
	fprintf(stderr, "Fuck 404!\n");
}


struct IRChain *IR_Exp_ID(void *root, struct Operand *place, struct TYPE *exp_type)
{
	if (place == NULL) return NULL;

	struct Exp_Variable	*ptr = (struct Exp_Variable *)root;

	struct Attribute	*attr = IRST_find(IRST, ptr -> var -> name) -> attr;

	if (attr -> type != Variable)
	{
		fprintf(stderr, "Fuck 024\n");
		return NULL;
	}

	struct VariableAttribute	*var = attr -> var;

	int no = get_var(var);

	struct Operand	*ret = NULL; 

	struct IRChain	*code = NULL;

	if (var -> type -> level == Basic)
	{
		ret = Build_Operand(VARIABLE, no);
	}
	else
	{
		struct Operand	*base = Build_Operand(VARIABLE, no);

		struct Operand	*ret_addr = Build_Operand(TEMP, get_temp());

		code = IRChain_link(code, IR_Address(var -> type, base, ret_addr, var -> is_ptr));

		if (Lvalue == 0)
		{
			ret = Build_Operand(TEMP, get_temp());
			code = IRChain_link(code, IRChain_build(Build_IRCode(AddressValueR, ret, ret_addr, NULL, 0, 0, NULL)));
		}
		else
			ret = ret_addr;

		if (var -> type -> level == Array)
		{
			IndexPtr = 0;
		}
		else
		{
//			fprintf(stderr, "%s: clear\n", ptr -> var -> name);
			Attribute_Offset = 0;
		}

	}

	if (place != NULL)
		code = IRChain_link(code, IRChain_build(Build_IRCode(ASSIGN, place, ret, NULL, 0, 0, NULL)));
	return code;
}

struct IRChain *IR_Exp_Assign(void *root, struct Operand *place, struct TYPE *exp_type)
{
	struct Exp_Assign		*ptr = (struct Exp_Assign *)root;

	struct Operand	*t1 = Build_Operand(TEMP, get_temp());
	struct IRChain	*codeA = IR_Exp(ptr -> exp_right, t1);

	struct Operand	*t2 = NULL;

	if (ptr -> exp_left -> IR == &IR_Exp_ID)	// UGLY
	{
		struct Exp_Variable		*L = (struct Exp_Variable *)(ptr -> exp_left -> next);
		t2 = ID_Operand(L -> var -> name, 0);
		struct IRChain	*codeB = IRChain_build(Build_IRCode(ASSIGN, t2, t1, NULL, 0, 0, NULL));
		codeA = IRChain_link(codeA, codeB);
	}
	else
	{
		t2 = Build_Operand(TEMP, get_temp());
		Lvalue = 1;
		struct IRChain	*codeB = IR_Exp(ptr -> exp_left, t2);
		Lvalue = 0;
		struct IRChain	*codeC = IRChain_build(Build_IRCode(AddressValueL, t2, t1, NULL, 0, 0, NULL));
		codeA = IRChain_link(codeA, codeB);
		codeA = IRChain_link(codeA, codeC);
	}

	if (place != NULL)
	{
		struct IRChain	*codeD = IRChain_build(Build_IRCode(ASSIGN, place, t2, NULL, 0, 0, NULL));
		codeA = IRChain_link(codeA, codeD);
	}
	
	return codeA;
}

struct IRChain *IR_Exp_Minus(void *root, struct Operand *place, struct TYPE *exp_type)
{
	if (place == NULL) return NULL;

	struct Exp_Unary	*ptr = (struct Exp_Unary *)root;

	struct Operand	*t1 = Build_Operand(TEMP, get_temp());

	struct IRChain	*codeA = IR_Exp(ptr -> exp, t1);
	struct IRChain	*codeB = IRChain_build(Build_IRCode(SUB, place, Zero, t1, 0, 0, NULL));

	return IRChain_link(codeA, codeB);
}

struct IRChain *IR_Exp_Par(void *root, struct Operand *place, struct TYPE *exp_type)
{
	if (place == NULL) return NULL;

	struct Exp_Unary	*ptr = (struct Exp_Unary *)root;

	struct IRChain	*code = IR_Exp(ptr -> exp, place);

	return code;
}

struct IRChain *IR_Exp_ADD_SUB_MUL_DIV(void *root, struct Operand *place, struct TYPE *exp_type)
{
	if (place == NULL) return NULL;

	struct Exp_Binary_Cal	*ptr = (struct Exp_Binary_Cal *)root;

	struct Operand	*t1 = Build_Operand(TEMP, get_temp());
	struct Operand	*t2 = Build_Operand(TEMP, get_temp());

	struct IRChain	*codeA = IR_Exp(ptr -> exp_a, t1);
	struct IRChain	*codeB = IR_Exp(ptr -> exp_b, t2);

	struct IRChain	*codeC;

	if (ptr -> op == OP_PLUS)
		codeC = IRChain_build(Build_IRCode(ADD, place, t1, t2, 0, 0, NULL));
	else if (ptr -> op == OP_MINUS)
		codeC = IRChain_build(Build_IRCode(SUB, place, t1, t2, 0, 0, NULL));
	else if (ptr -> op == OP_STAR)
		codeC = IRChain_build(Build_IRCode(MUL, place, t1, t2, 0, 0, NULL));
	else if (ptr -> op == OP_DIV)
		codeC = IRChain_build(Build_IRCode(DIVDIV, place, t1, t2, 0, 0, NULL));
	else
		fprintf(stderr, "Fuck 100\n");

	codeA = IRChain_link(codeA, codeB);
	codeA = IRChain_link(codeA, codeC);
	
	return codeA;
}


struct IRChain *IR_Exp_Function(void *root, struct Operand *place, struct TYPE *exp_type)
{
	struct Exp_Function		*ptr = (struct Exp_Function *)root;

	if (ptr -> args == NULL)
	{
		if (strcmp(ptr -> func -> name, "read") == 0)
		{
			if (place == NULL)
			{
				fprintf(stderr, "READ to void\n");
				return NULL;
			}
			return IRChain_build(Build_IRCode(READ, place, NULL, NULL, 0, 0, NULL));
		}
		if (place == NULL)
		{
			struct Operand	*t = Build_Operand(TEMP, get_temp());
			return IRChain_build(Build_IRCode(CALL, t, NULL, NULL, 0, 0, ptr -> func -> name));
		}
		else
			return IRChain_build(Build_IRCode(CALL, place, NULL, NULL, 0, 0, ptr -> func -> name));
	}
	else
	{
		struct ArgsChain	*list = NULL;

		struct IRChain *codeA = IR_Args(ptr -> args, &list);

		if (strcmp(ptr -> func -> name, "write") == 0)
		{
			struct IRChain *codeB = IRChain_build(Build_IRCode(WRITE, list -> arg, NULL, NULL, 0, 0, NULL));
			codeA = IRChain_link(codeA, codeB);
			if (place != NULL)
			{
				struct IRChain *codeC = IRChain_build(Build_IRCode(ASSIGN, place, Zero, NULL, 0, 0, NULL));
				codeA = IRChain_link(codeA, codeC);
			}
			return codeA;
		}
		//if (list == NULL) fprintf(stderr, "Fuck!\n");
		while (list)
		{
			struct IRChain *codeB = IRChain_build(Build_IRCode(ARG, list -> arg, NULL, NULL, 0, 0, NULL));
			codeA = IRChain_link(codeA, codeB);
			list = list -> next;
		}
		struct IRChain *codeB;
		if (place == NULL)
		{
			struct Operand	*t = Build_Operand(TEMP, get_temp());
			codeB = IRChain_build(Build_IRCode(CALL, t, NULL, NULL, 0, 0, ptr -> func -> name));
		}
		else
			codeB = IRChain_build(Build_IRCode(CALL, place, NULL, NULL, 0, 0, ptr -> func -> name));
		return IRChain_link(codeA, codeB);
	}
}

struct IRChain *IR_Exp_Attribute(void *root, struct Operand *place, struct TYPE *exp_type)
{
	struct Exp_Attribute	*ptr = (struct Exp_Attribute *)root;
	
	//index[IndexPtr].type = Offset_Attribute;
	struct StructureType	*head = ptr -> exp -> exp_type -> structure;
	//index[IndexPtr].key = Build_Operand(CONSTANT, offset(head, ptr -> attribute -> name));
	//++ IndexPtr;

	Attribute_Offset += offset(head, ptr -> attribute -> name);
	//fprintf(stderr, "offset = %d, total = %d\n", offset(head, ptr -> attribute -> name), Attribute_Offset);

	return IR_Exp(ptr -> exp, place);
}

struct IRChain *IR_Exp_Array(void *root, struct Operand *place, struct TYPE *exp_type)
{
	struct Exp_Array	*ptr = (struct Exp_Array *)root;

	ArrayIndex[IndexPtr] = Build_Operand(TEMP, get_temp());
	struct IRChain	*code = IR_Exp(ptr -> index, ArrayIndex[IndexPtr]);
	++ IndexPtr;

	return (IRChain_link(code, IR_Exp(ptr -> array, place)));
}

struct IRChain *IR_Args(struct Args *root, struct ArgsChain **list)
{
	struct IRChain	*ret = NULL;
	while (root)
	{
		struct Operand	*t = Build_Operand(TEMP, get_temp());
		struct IRChain	*code = NULL;
		if (root -> exp -> exp_type -> level == Basic)
		{
			code = IR_Exp(root -> exp, t);
		}
		else
		{
			Lvalue = 1;
			code = IR_Exp(root -> exp, t);
			Lvalue = 0;
		}

		struct ArgsChain	*new = (struct ArgsChain *)malloc(sizeof(struct ArgsChain));

		new -> arg = t;
		new -> next = *list;
		*list = new;

		ret = IRChain_link(ret, code);
		root = root -> args;
	}
	return ret;
}
