#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmm.h"

#define ENDLINE	fprintf(MC_file, "\n");

#define Min(X,Y)	(X < Y ? X : Y)
#define Infinity	100000000
/*
#define P(x)	fprintf(file, x); \
				ENDLINE
*/

#define	MaxRegister		22		// $t0-$t9, $s0-$s7, $a0-$a3
#define MaxStack	100000
#define MaxArg		100
// Bug: size !!!

#define FPoffset	8		// $fp + $ra

/* Available Registers */
struct RegisterType		Register[MaxRegister];
struct RegisterType		FP, SP, V0, V1, RA;

struct RegisterType		*A0;


struct IRChain	*pc = NULL;
FILE	*MC_file = NULL;


/* Stack Frame */
/* Static */
struct StackFrame		Stack[MaxStack];
int		StackPtr;
int		StackOffset;


/* Dynamic */
int		CallerArgsOffset;


/* Args Frame */
struct StackFrame		Argument[MaxArg];
int		ArgPtr;


/* ============= Argument ============== */
void Arg_clear(void)
{
	ArgPtr = 0;
}

int Arg_push(struct Operand *var)
{
	int ret;
	Argument[ArgPtr].key = var;
	Argument[ArgPtr].size = 4;
	ret = Argument[ArgPtr].offset = (ArgPtr + 1) * 4;

	++ ArgPtr;
	return ret;
}

int Arg_offset(struct Operand *var)
{
	int i;
	for (i = 0; i < ArgPtr; ++ i)
		if (Checking_Operand(Argument[i].key, var) == 1)
			return (Argument[i].offset);
	return -1;
}

void Arg_build(struct IRChain *head)
{
	Arg_clear();
	while (head)
	{
		struct IRCode	*code = head -> code;

		if (code -> type == PARAM)
			Arg_push(code -> single);

		head = head -> next;
	}
}


/* ============= Stack ============== */
void Stack_clear(void)
{
	StackPtr = StackOffset = 0;
}

int Stack_push(struct Operand *var, int size)
{
	int ret;
	Stack[StackPtr].key = var;	
	Stack[StackPtr].size = size;
	ret = Stack[StackPtr].offset = (StackOffset + size - 4);

	StackOffset += size;
	++ StackPtr;

	return ret;
}

int Stack_offset(struct Operand *var)
{
	int i;
	for (i = 0; i < StackPtr; ++ i)
		if (Stack[i].key != NULL && Checking_Operand(Stack[i].key, var) == 1)
			return (Stack[i].offset);
	return -1;
}

void Stack_update(struct Operand *var)
{
	if (Checking_Constant(var) == 1) return;
	if (Arg_offset(var) == -1 && Stack_offset(var) == -1)
		Stack_push(var, 4);
}

int Stack_build(struct IRChain *head)
{
	Stack_clear();
	while (head)
	{
		struct IRCode	*code = head -> code;

		if (code -> type == ASSIGN || code -> type == AddressValueL || code -> type == AddressValueR)
		{
			Stack_update(code -> lr.left);
			Stack_update(code -> lr.right);
		}
		else if (code -> type == ARG || code -> type == READ || code -> type == WRITE || code -> type == RETURNRETURN)
		{
			Stack_update(code -> single);
		}
		else if (code -> type == CALL)
		{
			Stack_update(code -> call.ret);
		}
		else if (code -> type == ADD || code -> type == SUB || code -> type == MUL || code -> type == DIVDIV)
		{
			Stack_update(code -> binary.result);
			Stack_update(code -> binary.op1);
			Stack_update(code -> binary.op2);
		}
		else if (code -> type == CONDGOTO)
		{
			Stack_update(code -> condjump.left);
			Stack_update(code -> condjump.right);
		}
		else if (code -> type == ValueAddress)
		{
			Stack_update(code -> lr.left);
		}
		else if (code -> type == DEC)
		{
			Stack_push(code -> dec.x, code -> dec.size);
		}
		head = head -> next;
	}
	return StackOffset;
}


/* ============= Register ============== */
void Register_special(void)
{
	sprintf(FP.name, "$fp");
	sprintf(RA.name, "$ra");
	sprintf(SP.name, "$sp");
	sprintf(V0.name, "$v0");
	sprintf(V1.name, "$v1");
	//sprintf(A0.name, "$a0");
	
	FP.key = RA.key = SP.key = V1.key = V0.key = NULL;

	A0 = Register + 18;

	int i;
	/* Register $t0 ~ $t9 */
	for (i = 0; i <= 9; ++ i)
	{
		sprintf(Register[i].name, "$t%d", i);
		Register[i].key = NULL;
	}

	/* Register $s0 ~ $s7 */
	for (i = 0; i <= 7; ++ i)
	{
		sprintf(Register[i + 10].name, "$s%d", i);
		Register[i + 10].key = NULL;
	}

	/* Register $a0 ~ $a3 */
	for (i = 0; i <= 3; ++ i)
	{
		sprintf(Register[i + 18].name, "$a%d", i);
		Register[i + 18].key = NULL;
	}
}

void Register_clear(void)
{
	int i;
	for (i = 0; i < MaxRegister; ++ i)
		Register[i].key = NULL;
}

void Register_update_operand(struct Operand *var, int time)
{
	struct RegisterType		*temp = Register_find(var);
	if (temp == NULL) return;
	temp -> next_use = Min(temp -> next_use, time);
}

void Register_update(void)
{
	int i;
	for (i = 0; i < MaxRegister; ++ i)
		Register[i].next_use = Infinity;
	
	struct IRChain	*ptr;

	int counter = 0;

	for (ptr = pc; ptr; ptr = ptr -> next, ++ counter)
	{
		struct IRCode *code = ptr -> code;
		if (code -> type == ASSIGN || code -> type == AddressValueL || code -> type == AddressValueR)
		{
			Register_update_operand(code -> lr.left, counter);
			Register_update_operand(code -> lr.right, counter);
		}
		else if (code -> type == ARG || code -> type == READ || code -> type == WRITE || code -> type == RETURNRETURN)
		{
			Register_update_operand(code -> single, counter);
		}
		else if (code -> type == CALL)
		{
			Register_update_operand(code -> call.ret, counter);
		}
		else if (code -> type == ADD || code -> type == SUB || code -> type == MUL || code -> type == DIVDIV)
		{
			Register_update_operand(code -> binary.result, counter);
			Register_update_operand(code -> binary.op1, counter);
			Register_update_operand(code -> binary.op2, counter);
		}
		else if (code -> type == CONDGOTO)
		{
			Register_update_operand(code -> condjump.left, counter);
			Register_update_operand(code -> condjump.right, counter);
		}
		else if (code -> type == ValueAddress)
		{
			Register_update_operand(code -> lr.left, counter);
		}
		// DEC, PARAM, LABEL, FUNCTION, GOTO ^_^
	}
}

struct RegisterType *Register_allocate(struct Operand *var)
{ 
	int i;
	for (i = 0; i < MaxRegister; ++ i)
		if (Register[i].key == NULL)
		{
			Register[i].key = var;
			return (Register + i);
		}

	/*
	for (i = 0; i < MaxRegister; ++ i)
		if (Checking_Constant(Register[i].key) == 1)
		{
			Register[i].key = var;
			return (Register + i);
		}
	*/

	//fprintf(stderr, "!!!!!!!!\n");
	Register_update();

	/*
	for (i = 0; i < MaxRegister; ++ i)
		fprintf(stderr, "%d ", Register[i].next_use);
	fprintf(stderr, "\n");
	*/
	
	int farthest = -1, next;
	for (i = 0; i < MaxRegister; ++ i)
		if (Register[i].key != NULL && Register[i].next_use > farthest)
		{
			next = i;
			farthest = Register[i].next_use;
		}

	//if (farthest != Infinity) // ^_^
	Register_spill(Register + next);

	Register[next].key = var;

	return (Register + next);
}

void Register_free(struct RegisterType *reg)
{
	//reg -> free = 1;
	reg -> key = NULL;
}

void Register_spill(struct RegisterType *reg)
{
	if (reg -> key == NULL) return;
	if (Checking_Constant(reg -> key) == 0)
	{
		int offset = Arg_offset(reg -> key);
		if (offset != -1)
			MC_Store(reg, offset, &FP);
		else
		{
			offset = Stack_offset(reg -> key);
			if (offset == -1)
				fprintf(stderr, "MC 010!\n");
			MC_Store(reg, -(offset + FPoffset), &FP);
		}	
	}

	Register_free(reg);
}

void Register_save(void)
{
	int i;
	for (i = 0; i < MaxRegister; ++ i)
		if (Register[i].key != NULL)
			Register_spill(Register + i);
}

char *Register_name(struct RegisterType *reg)
{
	return (reg -> name);
}

struct RegisterType *Register_find(struct Operand *var)
{
	int i;
	for (i = 0; i < MaxRegister; ++ i)
		if (Register[i].key != NULL && Checking_Operand(var, Register[i].key) == 1)
			return (Register + i);
	return NULL;
}

struct RegisterType *Register_get(struct Operand *var)
{
	struct RegisterType		*ret = Register_find(var);

	if (ret != NULL) return ret;
	
	ret = Register_allocate(var);

	// Load Variable
	if (Checking_Constant(var) == 0)
	{
		int offset = Arg_offset(var);
		if (offset != -1)	// From Argument
			MC_Load(&FP, offset, ret);
		else
		{
			offset = Stack_offset(var);
			if (offset == -1)	// From Local Variable
				fprintf(stderr, "MC 013!\n");
			MC_Load(&FP, -(offset + FPoffset), ret);
		}
	}

	return ret;
}


/* ============= Machine Code ============== */
void MC_Prologue(FILE *file)
{
	Register_special();

	MC_file = file;
	
	// Header
	fprintf(MC_file, ".data"); ENDLINE
	fprintf(MC_file, "_ret: .asciiz \"\\n\""); ENDLINE
	fprintf(MC_file, ".globl main"); ENDLINE
	fprintf(MC_file, ".text"); ENDLINE
	ENDLINE

	// Read
	fprintf(MC_file, "read:"); ENDLINE
	fprintf(MC_file, "li $v0, 5"); ENDLINE
	fprintf(MC_file, "syscall"); ENDLINE
	fprintf(MC_file, "jr $ra"); ENDLINE
	ENDLINE

	// Write
	fprintf(MC_file, "write:"); ENDLINE
	fprintf(MC_file, "li $v0, 1"); ENDLINE
	fprintf(MC_file, "syscall"); ENDLINE
	fprintf(MC_file, "li $v0, 4"); ENDLINE
	fprintf(MC_file, "la $a0, _ret"); ENDLINE
	fprintf(MC_file, "syscall"); ENDLINE
	fprintf(MC_file, "move $v0, $0"); ENDLINE
	fprintf(MC_file, "jr $ra"); ENDLINE
	ENDLINE
}

/* dest := *src */
void MC_Load(struct RegisterType *src, int offset, struct RegisterType *dest)
{
	fprintf(MC_file, "lw %s, %d(%s)", Register_name(dest), offset, Register_name(src)); ENDLINE
}

/* *dest := src */
void MC_Store(struct RegisterType *src, int offset, struct RegisterType *dest)
{
	fprintf(MC_file, "sw %s, %d(%s)", Register_name(src), offset, Register_name(dest)); ENDLINE
}

void MC_Move(struct RegisterType *src, struct RegisterType *dest)
{
	fprintf(MC_file, "move %s, %s", Register_name(dest), Register_name(src)); ENDLINE
}

void MC_SP(int offset)
{
	fprintf(MC_file, "addi $sp, $sp, %d", offset); ENDLINE
}

void MC_Save_fp(void)
{
	MC_SP(-4);
	MC_Store(&FP, 0, &SP);
	MC_Move(&SP, &FP);
}

void MC_Restore_fp(void)
{
	MC_Move(&FP, &SP);
	MC_Load(&FP, 0, &FP);
	MC_SP(4);
}

void MC_Save_ra(void)
{
	MC_SP(-4);
	MC_Store(&RA, 0, &SP);
}

void MC_Restore_ra(void)
{
	MC_Load(&FP, -4, &RA);
}

void MC_Li(struct RegisterType *dest, int key)
{
	fprintf(MC_file, "li %s, %d", Register_name(dest), key); ENDLINE
}

void MC_Label(struct IRCode *code)
{
	fprintf(MC_file, "L%d:", code -> label); ENDLINE
}

void MC_Function(struct IRCode *code)
{
	CallerArgsOffset = 0;
	Register_clear();

	fprintf(MC_file, "%s:", code -> func); ENDLINE
	MC_Save_fp();
	MC_Save_ra();

	/* Push all variable into stack */
	MC_SP(-StackOffset);
}

void MC_Param(struct IRCode *code)
{
	/* None */
}

void MC_Assign(struct IRCode *code)
{
	struct RegisterType		*left = Register_get(code -> lr.left);	
	if (Checking_Constant(code -> lr.right) == 1)
	{
		MC_Li(left, code -> lr.right -> constant);
	}
	else
	{
		struct RegisterType		*right = Register_get(code -> lr.right);	
		MC_Move(right, left);
	}
}

void MC_Calc(struct IRCode *code)
{
	struct RegisterType		*result = Register_get(code -> binary.result);

	int	constantA = Checking_Constant(code -> binary.op1);
	int	constantB = Checking_Constant(code -> binary.op2);

	struct RegisterType		*op1, *op2;
	if (constantA == 1)
	{
		MC_Li(&V0, code -> binary.op1 -> constant);
		op1 = &V0;
	}
	else
		op1 = Register_get(code -> binary.op1);

	if (constantB == 1)
	{
		MC_Li(&V1, code -> binary.op2 -> constant);
		op2 = &V1;
	}
	else
		op2 = Register_get(code -> binary.op2);

/*
		if (code -> type == ADD)
		{
			fprintf(MC_file, "addi %s, %s, %d", Register_name(result), Register_name(op1), code -> binary.op2 -> constant); ENDLINE
		}
		else if (code -> type == SUB)
		{
			fprintf(MC_file, "addi %s, %s, %d", Register_name(result), Register_name(op1), -(code -> binary.op2 -> constant)); ENDLINE
		}
		else
		{
			MC_Li(&V0, code -> binary.op2 -> constant);
			if (code -> type == MUL)
			{
				fprintf(MC_file, "mul %s, %s, %s", Register_name(result), Register_name(op1), Register_name(&V0)); ENDLINE
			}
			else
			{
				fprintf(MC_file, "div %s, %s", Register_name(op1), Register_name(&V0)); ENDLINE
					fprintf(MC_file, "mflo %s", Register_name(result)); ENDLINE
			}
		}
	struct RegisterType		*op1 = Register_get(code -> binary.op1);
	struct RegisterType		*op2 = Register_get(code -> binary.op2);

*/
	if (code -> type == ADD)
	{
		fprintf(MC_file, "add %s, %s, %s", Register_name(result), Register_name(op1), Register_name(op2)); ENDLINE
	}
	else if (code -> type == SUB)
	{
		fprintf(MC_file, "sub %s, %s, %s", Register_name(result), Register_name(op1), Register_name(op2)); ENDLINE
	}
	else if (code -> type == MUL)
	{
		fprintf(MC_file, "mul %s, %s, %s", Register_name(result), Register_name(op1), Register_name(op2)); ENDLINE
	}
	else	// DIVDIV
	{
		fprintf(MC_file, "div %s, %s", Register_name(op1), Register_name(op2)); ENDLINE
		fprintf(MC_file, "mflo %s", Register_name(result)); ENDLINE
	}
}

void MC_Goto(struct IRCode *code)
{
	Register_save();
	fprintf(MC_file, "j L%d", code -> label); ENDLINE
}

void MC_ValueAddress(struct IRCode *code)
{
	struct RegisterType		*left = Register_get(code -> lr.left);	
	int offset = Stack_offset(code -> lr.right);

	fprintf(MC_file, "addi %s, %s, %d", Register_name(left), Register_name(&FP), -(offset + FPoffset)); ENDLINE
}

void MC_AddressValueL(struct IRCode *code)
{
	struct RegisterType		*left = Register_get(code -> lr.left);	
	struct RegisterType		*right = Register_get(code -> lr.right);	

	MC_Store(right, 0, left);
}

void MC_AddressValueR(struct IRCode *code)
{
	struct RegisterType		*left = Register_get(code -> lr.left);	
	struct RegisterType		*right = Register_get(code -> lr.right);	

	MC_Load(right, 0, left);
}

void MC_Return(struct IRCode *code)
{
	struct RegisterType		*reg = Register_get(code -> single);

	MC_Move(reg, &V0);
	Register_save();
	MC_Restore_fp();

	fprintf(MC_file, "jr %s", Register_name(&RA)); ENDLINE
}

void MC_CondGoto(struct IRCode *code)
{
	Register_save();

	int	constantL = Checking_Constant(code -> condjump.left);
	int	constantR = Checking_Constant(code -> condjump.right);

	struct RegisterType		*left, *right;
	if (constantL == 1)
	{
		MC_Li(&V0, code -> condjump.left -> constant);
		left = &V0;
	}
	else
		left = Register_get(code -> condjump.left);	

	if (constantR == 1)
	{
		MC_Li(&V1, code -> condjump.right -> constant);
		right = &V1;
	}
	else
		right = Register_get(code -> condjump.right);	

	if (code -> condjump.op == OP_EQ)
		fprintf(MC_file, "beq ");
	else if (code -> condjump.op == OP_NE)
		fprintf(MC_file, "bne ");
	else if (code -> condjump.op == OP_LT)
		fprintf(MC_file, "blt ");
	else if (code -> condjump.op == OP_GT)
		fprintf(MC_file, "bgt ");
	else if (code -> condjump.op == OP_GE)
		fprintf(MC_file, "bge ");
	else if (code -> condjump.op == OP_LE)
		fprintf(MC_file, "ble ");
	else
		fprintf(stderr, "MC 000!\n");

	fprintf(MC_file, "%s, %s, L%d", Register_name(left), Register_name(right), code -> condjump.next); ENDLINE

	Register_free(left);
	Register_free(right);
}

void MC_Read(struct IRCode *code)
{
	fprintf(MC_file, "jal read"); ENDLINE
	MC_Restore_ra();

	struct RegisterType		*reg = Register_get(code -> single);
	MC_Move(&V0, reg);
}

void MC_Write(struct IRCode *code)
{
	struct RegisterType		*reg = Register_get(code -> single);

	Register_spill(A0);
	MC_Move(reg, A0);
	fprintf(MC_file, "jal write"); ENDLINE

	MC_Restore_ra();
}


void MC_Dec(struct IRCode *code)
{
	/* None */
}

void MC_Arg(struct IRCode *code)
{
	Register_save();

	struct RegisterType		*reg = Register_get(code -> single);

	CallerArgsOffset += 4;

	MC_SP(-4);	
	MC_Store(reg, 0, &SP);
	Register_free(reg);	// ^_^
}

void MC_Call(struct IRCode *code)
{
	Register_save();

	fprintf(MC_file, "jal %s", code -> call.func); ENDLINE
	MC_SP(CallerArgsOffset);
	CallerArgsOffset = 0;

	MC_Restore_ra();

	struct RegisterType		*reg = Register_get(code -> call.ret);
	MC_Move(&V0, reg);
}

void MC(struct IRCode *code)
{
	if (code -> type == LABEL)
	{
		MC_Label(code);
	}
	else if (code -> type == FUNCTION)
	{
		MC_Function(code);
	}
	else if (code -> type == ASSIGN)
	{
		MC_Assign(code);
	}
	else if (code -> type == ADD || code -> type == MUL || code -> type == SUB || code -> type == DIVDIV)
	{
		MC_Calc(code);
	}
	else if (code -> type == GOTO)
	{
		MC_Goto(code);
	}
	else if (code -> type == ValueAddress)
	{
		MC_ValueAddress(code);
	}
	else if (code -> type == AddressValueL)
	{
		MC_AddressValueL(code);
	}
	else if (code -> type == AddressValueR)
	{
		MC_AddressValueR(code);
	}
	else if (code -> type == CONDGOTO)
	{
		MC_CondGoto(code);
	}
	else if (code -> type == RETURNRETURN)
	{
		MC_Return(code);
	}
	else if (code -> type == READ)
	{
		MC_Read(code);
	}
	else if (code -> type == WRITE)
	{
		MC_Write(code);
	}
	else if (code -> type == DEC)
	{
		MC_Dec(code);
	}
	else if (code -> type == ARG)
	{
		MC_Arg(code);
	}
	else if (code -> type == CALL)
	{
		MC_Call(code);
	}
	else if (code -> type == PARAM)
	{
		MC_Param(code);
	}
	else
		fprintf(stderr, "MC 003!\n");
}

void GeneratingMC(struct IRChain *code)		// Block by Block
{
	//fprintf(MC_file, "==============================\n");
	Register_clear();

	pc = code;
	while (pc)
	{
		MC(pc -> code);	
		//if (pc -> next == code) break;
		pc = pc -> next;
	}

	Register_save();
}

