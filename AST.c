#include "const.h"
#include <stdlib.h>
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

	ptr -> tree.lineno = lineno;
	ptr -> extdeflist = child;
	
	return ptr;
}

struct ExtDefList *Build_ExtDefList(struct ExtDef *child_A, struct ExtDefList *child_B, int lineno)
{
	struct ExtDefList	*ptr = (struct ExtDefList *)malloc(sizeof(struct ExtDefList));

	ptr -> tree.lineno = lineno;
	ptr -> extdef = child_A;
	ptr -> extdeflist = child_B;

	return ptr;
}

struct ExtDef *Build_ExtDef(void *child, void (*func_visit)(struct ExtDef *), int lineno)
{
	struct ExtDef	*ptr = (struct ExtDef *)malloc(sizeof(struct ExtDef));

	ptr -> tree.lineno = lineno;
	ptr -> next = child;
	ptr -> Visit = func_visit;

	return ptr;
}

