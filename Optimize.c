#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmm.h"

#define Max(X,Y)	(X > Y ? X : Y)

/*
#define MaxMapping	1000

struct Operand	*Mapping_Var[MaxMapping];
struct Operand	*Mapping_Temp[MaxMapping];

int	VarSize;
int TempSize;

*/

#define MaxLabel	5000

struct CodeBlock	*BlockPool[MaxLabel];

struct NextBlock	*Candidate;

void IRChain_Delete(struct IRChain *ptr)
{
	ptr -> prev -> next = ptr -> next;
	ptr -> next -> prev = ptr -> prev;
}

//Add Code (new) following Code (ptr)
void IRChain_Insert(struct IRChain *ptr, struct IRChain *new)
{
	new -> next = ptr -> next;
	new -> prev = ptr;
	ptr -> next -> prev = new;
	ptr -> next = new;
}

void IRChain_Split(struct IRChain *A, struct IRChain *B)
{
	if (A == NULL || B == NULL) return;
	A -> prev -> next = B;
	B -> prev -> next = A;
	struct IRChain *temp = B -> prev;
	B -> prev = A -> prev;
	A -> prev = temp;
}


/*
void Update(struct Operand *ptr)
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
*/


int Checking_Constant(struct Operand *A)
{
	return ((A -> type == CONSTANT) ? 1 : 0);
}

int Checking_JUMP(struct IRCode *A)
{
	if (A -> type == GOTO)
		return (A -> label);
	else if (A -> type == CONDGOTO)
		return (A -> condjump.next);
	else
		return -1;
}

int Checking_Operand(struct Operand *A, struct Operand *B)
{
	return ((A -> type == B -> type && A -> var_no == B -> var_no) ? 1 : 0);
}

int Checking_DefaultJUMP(struct IRCode *A)
{
	return ((A -> type == GOTO || A -> type == RETURNRETURN) ? 0 : 1);
}

/*

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
		else if (A -> type == CONSTANT)
		{
			break;
		}
		else
			fprintf(stderr, "OPT 002!\n");
	}
	return A;
}

// A --> B
void Mapping_Replace(struct Operand *A, struct Operand *B, struct IRChain *ptr)
{
	if (A -> type != VARIABLE && A -> type != TEMP)
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

int IR_Optimizer_A(struct IRChain *code)
{
	int flag = 0;
	struct IRChain	*ptr = code;
	struct IRChain	*next;
	struct IRCode	*x;
	while (1)
	{
		//printf("%d:===============================================\n", flag ++);
		//Print_IRChain(code, stderr);
		next = ptr -> next;
		x = ptr -> code;

		Print_IRCode(x, stdout);

		if (x -> type == ASSIGN)
		{
			Update(x -> lr.left); Update(x -> lr.right);
			Mapping_Replace(x -> lr.left, Mapping_Source(x -> lr.right), ptr);
			if (Checking_Constant(x -> lr.right) == 0)
				IRChain_Delete(ptr);
		}
		else if (x -> type == ADD || x -> type == SUB || x -> type == MUL || x -> type == DIVDIV)
		{
			Update(x -> binary.result); Update(x -> binary.op1); Update(x -> binary.op2);
			Mapping_Replace(x -> binary.result, NULL, ptr);
		}
		else if (x -> type == ValueAddress || x -> type == AddressValueR)
		{
			Update(x -> lr.left); Update(x -> lr.right);
			Mapping_Replace(x -> lr.left, NULL, ptr);
		}
		if (next == code) break;
		ptr = next;
	}
	return 0;
}

*/

inline int Get_Label(struct IRChain *ptr)
{
	if (ptr -> code -> type == LABEL)
		return ptr -> code -> label;
	else
		return 0;
}

struct IRChain	*Get_Block(struct IRChain **head)
{
	if ((*head) == NULL) return NULL;

	struct IRChain	*ptr = (*head) -> next;

	struct IRChain	*ret = (*head);

	while (ptr != (*head))
	{
		if (ptr -> code -> type == LABEL)
		{
			IRChain_Split(*head, ptr);	
			*head = ptr;
			return ret;
		}
		ptr = ptr -> next;
	}
	*head = NULL;
	return ret;
}


void CFG_print(FILE *file)
{
	struct NextBlock	*iterator = Candidate;
	while (iterator)
	{
		struct NextBlock	*ptr = iterator -> target -> out;
		while (ptr)
		{
			fprintf(file, "%d -> %d\n", iterator -> target -> label, ptr -> target -> label);
			ptr = ptr -> next;
		}
		iterator = iterator -> next;
	}
}

void CFG_insert(struct CodeBlock *A, struct CodeBlock *B)
{
	struct NextBlock	*ptr = (struct NextBlock *)malloc(sizeof(struct NextBlock));;

	ptr -> target = B;
	ptr -> next = A -> out;
	A -> out = ptr;
}

void CFG_build(struct CodeBlock *ptr)
{
	struct IRChain		*code = ptr -> code;
	struct IRChain		*currentcode = code -> next;	// first line must be "FUNCTION" or LABEL"
	while (currentcode != code)
	{
		int target = Checking_JUMP(currentcode -> code);

		if (target != -1)
			CFG_insert(ptr, BlockPool[target]);		

		currentcode = currentcode -> next;
	}
}

struct IRChain	*OPT_RETURN(struct IRChain *head)
{
	//remove ALL the code following the RETURN instruction in a block
}

struct IRChain	*OPT_Function(struct IRChain *head, FILE *file)
{
	// Initialize the Block Pool
	int i;
	for (i = 0; i < MaxLabel; ++ i)
		BlockPool[i] = NULL;

	// Divide function into blocks
	struct IRChain	*ptr;

	struct CodeBlock	*waitting = NULL;

	Candidate = NULL;

	while ((ptr = Get_Block(&head)) != NULL)
	{

		/* For Generating Machine Code */

		ptr -> prev -> next = NULL; // Break Code Ring

		GeneratingMC(ptr);
		
		ptr -> prev -> next = ptr; // Link Code Ring

		/* For Optimizing IR Code */

		fprintf(file, "==========Block=========\n");
		Print_IRChain(ptr, file);
		
		int label = Get_Label(ptr);

		if (BlockPool[label] != NULL)
		{
			fprintf(stderr, "OPT 000!\n");
			return NULL;
		}

		struct CodeBlock	*new = (struct CodeBlock *)malloc(sizeof(struct CodeBlock));
		new -> label = label;
		new -> code = ptr;
		new -> out = NULL;

		// Candidate
		struct NextBlock	*list = (struct NextBlock *)malloc(sizeof(struct NextBlock));
		list -> next = Candidate;
		list -> target = new;
		Candidate = list;

		// Set default 
		if (waitting != NULL)
		{
			CFG_insert(waitting, new);
			waitting = NULL;
		}

		if (Checking_DefaultJUMP(ptr -> prev -> code) == 1)
			waitting = new;

		BlockPool[label] = new;

		//TotalBlock = Max(TotalBlock, label);
		
		//OPT_Block(func_head);
	}

	// Create block graph
	struct NextBlock	*iterator = Candidate;
	while (iterator)
	{
		CFG_build(iterator -> target);
		iterator = iterator -> next;
	}

	// Test block graph
	CFG_print(file);

	// Free Candidate
	iterator = Candidate;
	while (iterator)
	{
		struct NextBlock	*next;
		next = iterator -> next;
		free(iterator);
		iterator = next;
	}
	Candidate = NULL;

	return NULL;
}

struct IRChain	*Get_Function(struct IRChain **head)
{
	if ((*head) == NULL) return NULL;

	if ((*head) -> code -> type != FUNCTION)
	{
		fprintf(stderr, "OPT 005!\n");
		return NULL;
	}
	struct IRChain	*ptr = (*head) -> next;

	struct IRChain	*ret = (*head);

	while (ptr != (*head))
	{
		if (ptr -> code -> type == FUNCTION)
		{
			IRChain_Split(*head, ptr);	
			*head = ptr;
			return ret;
		}
		ptr = ptr -> next;
	}
	*head = NULL;
	return ret;
}

struct IRChain	*IR_Optimizer(struct IRChain *code, FILE *file)
{
	struct IRChain	*func_head;
	while ((func_head = Get_Function(&code)) != NULL)
	{
		fprintf(file, "==========Function=========\n");
		//Print_IRChain(func_head, stdout);
		OPT_Function(func_head, file);
	}
	/*
	int i;
	for (i = 0; i < MaxMapping; ++ i)
	{

		Mapping_Var[i] = NULL;
		Mapping_Temp[i] = NULL;
	}
	VarSize = TempSize = -1;

	IR_Optimizer_A(code);

	Print_IRChain(code, stderr);
	*/
}

