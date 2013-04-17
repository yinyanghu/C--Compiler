#include "SymbolsTable.h"
#include <stdlib.h>
#include <string.h>

struct SymbolsTableID		*STID[HashingPrime];
struct SymbolsTableNumber	*STNUM[HashingPrime];


// BKDR Hash Function
int StringHashing(char *str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	while (*str)
		hash = hash * seed + (*str ++);
	return ((hash & 0x7FFFFFFF) % HashingPrime);
}

struct SymbolsTableID *STID_insert(struct SymbolsTableID **List, char *id)
{
	struct SymbolsTableID *ptr;
	if ((ptr = STID_find(List, id)) != NULL) return ptr;

	ptr = (struct SymbolsTableID *)malloc(sizeof(struct SymbolsTableID));
	strcpy(ptr -> name, id);

	int f = StringHashing(id);
	ptr -> next = List[f] -> next;
	List[f] -> next = ptr;

	return ptr;
}

struct SymbolsTableID *STID_find(struct SymbolsTableID **List, char *id)
{
	int f = StringHashing(id);
	struct SymbolsTableID	*ptr;
	for (ptr = List[f]; ptr != NULL; ptr = ptr -> next)
		if (strcmp(id, ptr -> name) == 0) return ptr;
	return NULL;
}

/* Just for Fun! */
int STID_Test(struct SymbolsTableID **List)
{
	int i, ret = 0;
	struct SymbolsTableID	*ptr;
	for (i = 0; i < HashingPrime; ++ i)
	{
		int len = 0;
		for (ptr = List[i]; ptr != NULL; ptr = ptr -> next)
			++ len;
		if (len > ret) ret = len;
	}
	return ret;
}


int IntHashing(int key)
{
	return (key % HashingPrime);
}


int FloatHashing(float key)
{
	return ((*((int *)&key)) % HashingPrime);
}

struct Constant Make_Constant(DataType type, int int_key, float float_key)
{
	struct Constant ret;
	ret.type = type;
	if (type == TYPE_INT)
		ret.v_int = int_key;
	else
		ret.v_float = float_key;
	return ret;
}

struct SymbolsTableNumber *STNUM_insert(struct SymbolsTableNumber **List, struct Constant num)
{
	struct SymbolsTableNumber *ptr;
	if ((ptr = STNUM_find(List, num)) != NULL) return ptr;

	ptr = (struct SymbolsTableNumber *)malloc(sizeof(struct SymbolsTableNumber));
	ptr -> value = num;

	int f = (num.type == TYPE_INT) ? IntHashing(num.v_int) : FloatHashing(num.v_float);
	ptr -> next = List[f] -> next;
	List[f] -> next = ptr;

	return ptr;
}


struct SymbolsTableNumber *STNUM_find(struct SymbolsTableNumber **List, struct Constant num)
{
	struct SymbolsTableNumber	*ptr;
	if (num.type == TYPE_INT)
	{
		int f = IntHashing(num.v_int);
		for (ptr = List[f]; ptr != NULL; ptr = ptr -> next)
			if (ptr -> value.v_int == num.v_int) return ptr;
	}
	else
	{
		int f = FloatHashing(num.v_float);
		for (ptr = List[f]; ptr != NULL; ptr = ptr -> next)
			if (ptr -> value.v_float == num.v_float) return ptr;
	}
	return NULL;
}

void SymbolsTableID_clear(struct SymbolsTableID **List)
{
	int i;
	for (i = 0; i < HashingPrime; ++ i)
		List[i] = NULL;
}

void SymbolsTableNumber_clear(struct SymbolsTableNumber **List)
{
	int i;
	for (i = 0; i < HashingPrime; ++ i)
		List[i] = NULL;
}
