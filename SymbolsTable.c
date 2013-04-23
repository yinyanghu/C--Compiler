#include "SymbolsTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SymbolsTable			*ST[HashingPrime];
struct ScopeType			Scope;


struct SymbolsTable			*SST[HashingPrime];
struct ScopeType			StructScope;


struct DynamicStructureTable	*DST;
int		DST_Scope;

struct DeclaredFunctionList		*DFL;


void DFL_clear(void)
{
	DFL = NULL;
}

struct DeclaredFunctionList *DFL_insert(struct DeclaredFunctionList *dfl, char *name, int lineno)
{
	struct DeclaredFunctionList		*ptr = (struct DeclaredFunctionList *)malloc(sizeof(struct DeclaredFunctionList));

	ptr -> next = dfl;
	ptr -> lineno = lineno;
	strcpy(ptr -> name, name);

	return ptr;
}

void DFL_remove(struct DeclaredFunctionList **dfl, char *name)
{
	struct DeclaredFunctionList		**ptr;
	for (ptr = dfl; *ptr; )
	{
		struct DeclaredFunctionList		*entry = *ptr;
		if (strcmp(entry -> name, name) == 0)
		{
			*ptr = entry -> next;
			free(entry);
		}
		else
			ptr = &entry -> next;
	}
}

int DFL_check(struct DeclaredFunctionList *dfl)
{
	return (dfl == NULL ? 1 : 0);
}


// BKDR Hash Function
int StringHashing(char *str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	while (*str)
		hash = hash * seed + (*str ++);
	return ((hash & 0x7FFFFFFF) % HashingPrime);
}

struct SymbolsTable *ST_insert(struct SymbolsTable **List, char *id, struct ScopeType *Scope)
{
	struct SymbolsTable *ptr = (struct SymbolsTable *)malloc(sizeof(struct SymbolsTable));
	//if ((ptr = ST_find(List, id)) != NULL) return ptr;

	strcpy(ptr -> name, id);
	ptr -> scope = Scope_current(Scope);
	ptr -> attr = NULL;

	int f = StringHashing(id);
	ptr -> next = List[f] -> next;
	ptr -> prev = List[f];
	if (List[f] -> next != NULL)
		List[f] -> next -> prev = ptr;
	List[f] -> next = ptr;

	Scope_insert(Scope, ptr);

	return ptr;
}

void ST_remove(struct SymbolsTable *ptr)
{
	ptr -> prev -> next = ptr -> next;
	if (ptr -> next != NULL)
		ptr -> next -> prev = ptr -> prev;
	free(ptr);
}

struct SymbolsTable *ST_find(struct SymbolsTable **List, char *id)
{
	int f = StringHashing(id);
	struct SymbolsTable	*ptr;

	for (ptr = List[f] -> next; ptr != NULL; ptr = ptr -> next)
		if (strcmp(id, ptr -> name) == 0) return ptr;
	return NULL;
}

struct SymbolsTable *ST_find_scope(struct SymbolsTable **List, char *id, struct ScopeType *scope)
{
	int current = Scope_current(scope);

	int f = StringHashing(id);
	struct SymbolsTable *ptr;

	for (ptr = List[f] -> next; ptr != NULL; ptr = ptr -> next)
		if (ptr -> scope == current && strcmp(id, ptr -> name) == 0) return ptr;
	return NULL;
}

/* Just for Fun! */
int ST_Test(struct SymbolsTable **List)
{
	int i, ret = 0;
	struct SymbolsTable	*ptr;
	for (i = 0; i < HashingPrime; ++ i)
	{
		int len = 0;
		for (ptr = List[i] -> next; ptr != NULL; ptr = ptr -> next)
			++ len;
		if (len > ret) ret = len;
	}
	return ret;
}

void ST_clear(struct SymbolsTable **List)
{
	int i;
	for (i = 0; i < HashingPrime; ++ i)
	{
		List[i] = (struct SymbolsTable *)malloc(sizeof(struct SymbolsTable));	//	Just a Head Pointer
		List[i] -> next = List[i] -> prev = NULL;
	}
}


void Scope_clear(struct ScopeType *scope)
{
	scope -> top = 0;
}

int Scope_current(struct ScopeType *scope)
{
	return scope -> top;
}

void Scope_push(struct ScopeType *scope)
{
	scope -> Stack[scope -> top] = NULL;
	++ scope -> top;
}

void Scope_pop(struct ScopeType *scope)
{
	if (scope -> top == 0)
	{
		fprintf(stderr, "Stack Error!\n");
		return;
	}
	struct ScopeList	*ptr = scope -> Stack[scope -> top - 1];
	while (ptr)
	{
		struct ScopeList	*next = ptr -> next;
		ST_remove(ptr -> pearl);
		free(ptr);
		ptr = next;
	}
	-- scope -> top;
}

void Scope_insert(struct ScopeType *scope, struct SymbolsTable *pearl)
{
	if (scope -> top == 0)
	{
		fprintf(stderr, "Stack Error!\n");
		return;
	}
	struct ScopeList	*ptr = (struct ScopeList *)malloc(sizeof(struct ScopeList));

	ptr -> pearl = pearl;

	ptr -> next = scope -> Stack[scope -> top - 1];
	scope -> Stack[scope -> top - 1] = ptr;
}


void ScopeList_print(struct ScopeList *list)
{
	struct SymbolsTable		*entry;
	for (; list; list = list -> next)
	{
		entry = list -> pearl;
		printf("%s(%d): \n", entry -> name, entry -> scope);
		Attribute_print(entry -> attr);
	}
}

void Scope_print(struct ScopeType *scope)
{
	int i;
	for (i = 0; i < scope -> top; ++ i)
	{
		printf("Scope %d: \n", i);
		ScopeList_print(scope -> Stack[i]);
	}
}

// NO
int DST_check(struct DynamicStructureTable *dst, struct StructureType *attr, char *name)
{
	struct DynamicStructureTable *ptr;		// ptr --> useless
	for (ptr = dst; ptr != NULL; ptr = ptr -> next)
		if (strcmp(ptr -> name, name) == 0) return 0;
	return 1;
}

// YES
struct DynamicStructureTable *DST_insert(struct DynamicStructureTable *dst, char *name, struct StructureType *attr)
{
	struct DynamicStructureTable	*ptr = (struct DynamicStructureTable *)malloc(sizeof(struct DynamicStructureTable));
	ptr -> next = dst;

	strcpy(ptr -> name, name);
	ptr -> attr = attr;
	ptr -> scope = DST_Scope;

	return ptr;
}

// YES
void DST_remove(struct DynamicStructureTable **dst, int scope)
{
	struct DynamicStructureTable	**ptr;
	for (ptr = dst; *ptr; )
	{
		struct DynamicStructureTable	*entry = *ptr;
		if (entry -> scope == scope)
		{
			*ptr = entry -> next;
			free(entry);
		}
		else
			ptr = &entry -> next;
	}
}

struct DynamicStructureTable *DST_find(struct DynamicStructureTable *dst, char *name)
{
	struct DynamicStructureTable *ptr;		// ptr --> useless
	for (ptr = dst; ptr != NULL; ptr = ptr -> next)
		if (strcmp(ptr -> name, name) == 0) return ptr;
	return NULL;
}

void DST_Global(void)
{
	DST_Scope = GlobalScope;
	DST_remove(&DST, LocalScope);
}

void DST_Local(void)
{
	DST_Scope = LocalScope;
}


void DST_clear(void)
{
	DST = NULL;	
	DST_Global();
}

void DST_print(struct DynamicStructureTable *dst)
{
	struct DynamicStructureTable *ptr;
	for (ptr = dst; ptr != NULL; ptr = ptr -> next)
	{
		printf("Name: %s, Scope: %d\n", ptr -> name, ptr -> scope);
		Structure_print(ptr -> attr);
		printf("\n");
	}

}



/*

int IntHashing(int key)
{
	return (key % HashingPrime);
}


int FloatHashing(float key)
{
	return ((*((int *)&key)) % HashingPrime);
}

*/
