#include <stdio.h>
#include <string.h>
#include "cmm.h"

#define Max(X,Y)	(X > Y ? X : Y)

#define MaxMapping	1000

struct Operand	*Mapping_Var[MaxMapping];
struct Operand	*Mapping_Temp[MaxMapping];

int	VarSize;
int TempSize;

void IRChain_Delete(struct IRChain *ptr)
{
	ptr -> prev -> next = ptr -> next;
	ptr -> next -> prev = ptr -> prev;
}

void update(struct Operand *ptr)
{
	if (ptr -> type == VARIABLE)
	{
		VarSize = Max(VarSize, ptr -> var_no);
	}
	else if (ptr -> type == TEMP)
	{
		TempSize = Max(TempSize, ptr -> temp_no);
	}
}

//Add Code (new) following Code (ptr)
void IRChain_Insert(struct IRChain *ptr, struct IRChain *new)
{
	new -> next = ptr -> next;
	new -> prev = ptr;
	ptr -> next -> prev = new;
	ptr -> next = new;
}

inline int Checking_Operand(struct Operand *A, struct Operand *B)
{
	return ((A -> type == B -> type && A -> var_no == B -> var_no) ? 1 : 0);
}

void Mapping_Store(struct Operand *X, struct IRChain *ptr)
{
	int i;
	for (i = 0; i < VarSize; ++ i)
		if (Mapping_Var[i] != NULL && Checking_Operand(Mapping_Var[i], X) == 1)
		{
			IRChain_Insert(ptr, IRChain_build(Build_IRCode(ASSIGN, Build_Operand(VARIABLE, i), X, NULL, 0, 0, NULL)));
			Mapping_Var[i] = NULL;
		}

	for (i = 0; i < TempSize; ++ i)
		if (Mapping_Temp[i] != NULL && Checking_Operand(Mapping_Temp[i], X) == 1)
		{
			IRChain_Insert(ptr, IRChain_build(Build_IRCode(ASSIGN, Build_Operand(TEMP, i), X, NULL, 0, 0, NULL)));
			Mapping_Temp[i] = NULL;
		}

}

struct Operand *Mapping_Source(struct Operand *A)
{
	while (1)
	{
		if (A -> type == VARIABLE)
		{
			if (Mapping_Var[A -> var_no] == NULL)
				break;
			A = Mapping_Var[A -> var_no];
		}
		else if (A -> type == TEMP)
		{
			if (Mapping_Temp[A -> temp_no] == NULL)
				break;
			A = Mapping_Temp[A -> temp_no];
		}
		else
			fprintf(stderr, "OPT 002!\n");
	}
	return A;
}

// A --> B
void Mapping_Replace(struct Operand *A, struct Operand *B, struct IRChain *ptr)
{
	if (A -> type != VARIABLE || A -> type != TEMP)
	{
		fprintf(stderr, "OPT 001!\n");
		return;
	}
	if (A -> type == VARIABLE)
	{
		int Index = A -> var_no;
		Mapping_Store(A, ptr);
		Mapping_Var[Index] = B;
	}
	else
	{
		int Index = A -> temp_no;
		Mapping_Store(A, ptr);
		Mapping_Temp[Index] = B;
	}
}

int IR_Optimizer_Assign(struct IRChain *code)
{
	int flag = 0;
	struct IRChain	*ptr = code;
	struct IRCode	*x;
	while (ptr)
	{
		x = ptr -> code;
		if (x -> type == ASSIGN)
		{
			Update(x -> lr.left); Update(x -> lr.right);
			Mapping_Replace(x -> lr.left, Mapping_Source(x -> lr.right));
		}
		else if (x -> type == ADD || x -> type == SUB || x -> type == MUL || x -> type == DIVDIV)
		{
			Update(x -> lr.left); Update(x -> lr.right);
			Mapping_Replace(x -> lr.left, Mapping_Source(x -> lr.right));
		}
		else if (x -> type == ValueAddress || x -> type == AddressValueR)
		{

		}
		ptr = ptr -> next;
	}
	
}

struct IRChain	*IR_Optimizer(struct IRChain *code)
{
	int i;
	for (i = 0; i < MaxMapping; ++ i)
	{
		Mapping_Var[i] = NULL:
		Mapping_Temp[i] = NULL:
	}
	VarSize = TempSize = -1;
}


