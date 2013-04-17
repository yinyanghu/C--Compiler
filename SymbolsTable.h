#ifndef SYMBOLSTABLE_H
#define SYMBOLSTABLE_H

#include "const.h"

#define IDNameSize	30
#define HashingPrime	997

struct Constant
{
	DataType	type;	
	union
	{
		int		v_int;
		float	v_float;
	};
};

struct SymbolsTableID
{
	char name[IDNameSize];
	struct SymbolsTableID	*next;
};

struct SymbolsTableNumber
{
	struct Constant		value;
	struct SymbolsTableNumber	*next;
};

int IntHashing(int);

int FloatHashing(float);

int StringHashing(char *);

struct Constant Make_Constant(DataType, int, float);

struct SymbolsTableID *STID_insert(struct SymbolsTableID **, char *);

struct SymbolsTableID *STID_find(struct SymbolsTableID **, char *);

void SymbolsTableID_clear(struct SymbolsTableID **);

struct SymbolsTableNumber *STNUM_insert(struct SymbolsTableNumber **, struct Constant);

struct SymbolsTableNumber *STNUM_find(struct SymbolsTableNumber **, struct Constant);

void SymbolsTableNumber_clear(struct SymbolsTableNumber **);

int STID_Test(struct SymbolsTableID **);
int STNUM_Test(struct SymbolsTableNumber **);

extern struct SymbolsTableID		*STID[HashingPrime];
extern struct SymbolsTableNumber	*STNUM[HashingPrime];

#endif
