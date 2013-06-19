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

#define	MaxRegister		18
#define MaxStack	100000
#define MaxArg		2000
// Bug: size !!!

#define FPoffset	8		// $fp + $ra

/* Available Registers */
struct RegisterType		Register[MaxRegister];
struct RegisterType		FP, SP, V0, V1, RA, A0;
int		RegisterSaved;


struct IRChain	*pc = NULL;
FILE	*MC_file = NULL;


/* Stack Frame */
struct StackFrame		Stack[MaxStack];
int		StackPtr;
int		StackOffset;
int		CallerArgsOffset;


/* Args Frame */
struct StackFrame		Argument[MaxArg];
int		ArgPtr;
//int		CalleeArgsOffset;


void Arg_clear(void)
{
//	CalleeArgsOffset = 0;
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

void Stack_clear(void)
{
	StackPtr = StackOffset = 0;
	CallerArgsOffset = 0;
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
		if (Checking_Operand(Stack[i].key, var) == 1)
			return (Stack[i].offset);
	return -1;
}

void Register_special(void)
{
	sprintf(FP.name, "$fp");
	sprintf(RA.name, "$ra");
	sprintf(SP.name, "$sp");
	sprintf(V0.name, "$v0");
	sprintf(V1.name, "$v1");
	sprintf(A0.name, "$a0");
	
	FP.key = RA.key = SP.key = V1.key = V0.key = A0.key = NULL;

	int i;
	// Register $t0 ~ $t9
	for (i = 0; i <= 9; ++ i)
	{
		sprintf(Register[i].name, "$t%d", i);
		//Register[i].free = 1;
		Register[i].key = NULL;
	}

	// Register $s0 ~ $s9
	for (i = 0; i <= 7; ++ i)
	{
		sprintf(Register[i + 10].name, "$s%d", i);
		//Register[i + 10].free = 1;
		Register[i + 10].key = NULL;
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
		else if (code -> type == ARG || code -> type == PARAM || code -> type == READ || code -> type == WRITE || code -> type == RETURNRETURN)
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
		// DEC, ValueAddress
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

	//if (farthest != Infinity) // ???
	Register_spill(Register + next);

	Register[next].key = var;

	return (Register + next);
}

void Register_free(struct RegisterType *reg)
{
	//reg -> free = 1;
	reg -> key = NULL;
}

void MC_SP(int offset)
{
	fprintf(MC_file, "addi $sp, $sp, %d", offset); ENDLINE
}

void Register_spill(struct RegisterType *reg)
{
	//fprintf(stderr, "******\n");
	int offset = Arg_offset(reg -> key);
	if (offset != -1)
	{
		MC_Store(reg, offset, &FP);
	}
	else
	{
		offset = Stack_offset(reg -> key);
		if (offset == -1)
		{
			offset = Stack_push(reg -> key, 4);
			MC_SP(-4);
		}

		MC_Store(reg, -(offset + FPoffset), &FP);
	}

	Register_free(reg);
}

void Register_save(void)
{
	if (RegisterSaved == 1) return;

	int i;
	for (i = 0; i < MaxRegister; ++ i)
		if (Register[i].key != NULL)
			Register_spill(Register + i);
	RegisterSaved = 1;
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

	int offset = Stack_offset(var);

	if (offset != -1)	// From Local Variable
		MC_Load(&FP, -(offset + FPoffset), ret);
	else
	{
		offset = Arg_offset(var);
		if (offset != -1)	// From Argument
			MC_Load(&FP, offset, ret);
	}

	return ret;
}

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
	Register_clear();
	Stack_clear();
	Arg_clear();

	fprintf(MC_file, "%s:", code -> func); ENDLINE
	MC_Save_fp();
	MC_Save_ra();
}

void MC_Param(struct IRCode *code)
{
	Arg_push(code -> single);
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

	int		constantA = Checking_Constant(code -> binary.op1);
	int		constantB = Checking_Constant(code -> binary.op2);

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

	struct RegisterType		*left = Register_get(code -> condjump.left);	
	struct RegisterType		*right = Register_get(code -> condjump.right);	

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
}

void MC_Read(struct IRCode *code)
{
	struct RegisterType		*reg = Register_get(code -> single);
	fprintf(MC_file, "jal read"); ENDLINE
	MC_Move(&V0, reg);
	MC_Restore_ra();
}

void MC_Write(struct IRCode *code)
{
	struct RegisterType		*reg = Register_get(code -> single);
	MC_Move(reg, &A0);
	fprintf(MC_file, "jal write"); ENDLINE
	MC_Restore_ra();
}


void MC_Dec(struct IRCode *code)
{
	Stack_push(code -> dec.x, code -> dec.size);
	MC_SP(-(code -> dec.size));
}

void MC_Arg(struct IRCode *code)
{
	if (RegisterSaved == 0)
		Register_save();

	struct RegisterType		*reg = Register_get(code -> single);

	CallerArgsOffset += 4;

	MC_SP(-4);	
	MC_Store(reg, 0, &SP);
}

void MC_Call(struct IRCode *code)
{
	if (RegisterSaved == 0)
		Register_save();

	fprintf(MC_file, "jal %s", code -> call.func); ENDLINE
	struct RegisterType		*reg = Register_get(code -> call.ret);
	MC_Move(&V0, reg);

	MC_SP(CallerArgsOffset);
	CallerArgsOffset = 0;

	MC_Restore_ra();
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
	Register_clear();
	RegisterSaved = 0;

	pc = code;
	while (pc)
	{
		MC(pc -> code);	
		//if (pc -> next == code) break;
		pc = pc -> next;
	}
	if (RegisterSaved == 0)
		Register_save();
}

