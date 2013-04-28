#ifndef SYMBOLSTABLE_H
#define SYMBOLSTABLE_H

#include "const.h"
#include "SemanticChecker.h"

#define HashingPrime	997
#define MaxScope		15
#define GlobalScope		0
#define LocalScope		1

/*
struct Constant {
	DataType	type;	
	union {
		int		v_int;
		float	v_float;
	};
};
*/
struct ScopeList {
	struct SymbolsTable	*pearl;
	struct ScopeList	*next;
};

struct ScopeType {
	int top;
	struct ScopeList	*Stack[MaxScope];
};

void Scope_clear(struct ScopeType *);

void Scope_push(struct ScopeType *);

void Scope_pop(struct ScopeType *);

void Scope_insert(struct ScopeType *, struct SymbolsTable *);

int Scope_current(struct ScopeType *);

void Scope_print(struct ScopeType *);

struct SymbolsTable {
	int scope;
	char name[NameSize];
	struct Attribute	*attr;
	struct SymbolsTable	*next, *prev;
};

struct DynamicStructureTable {
	int scope;		// 0 --- global or 1 -- local
	char name[NameSize];
	struct StructureType	*attr;
	struct DynamicStructureTable *next;
};

struct DeclaredFunctionList {
	int lineno;
	char name[NameSize];
	struct DeclaredFunctionList	*next;
};

/*
int IntHashing(int);

int FloatHashing(float);

struct Constant Make_Constant(DataType, int, float);
*/

void DFL_clear(void);

struct DeclaredFunctionList *DFL_insert(struct DeclaredFunctionList *, char *, int);

void DFL_remove(struct DeclaredFunctionList **, char *);

int DFL_check(struct DeclaredFunctionList *);




int StringHashing(char *);

struct SymbolsTable *ST_insert(struct SymbolsTable **, char *, struct ScopeType *);

struct SymbolsTable *ST_find(struct SymbolsTable **, char *);

struct SymbolsTable *ST_find_scope(struct SymbolsTable **, char *, struct ScopeType *);

void ST_remove(struct SymbolsTable *);

void ST_clear(struct SymbolsTable **);

int ST_Test(struct SymbolsTable **);


struct DynamicStructureTable *DST_insert(struct DynamicStructureTable *, char *, struct StructureType *);

void DST_remove(struct DynamicStructureTable **, int);

struct DynamicStructureTable *DST_find(struct DynamicStructureTable *, char *);

int DST_check_name(struct DynamicStructureTable *, char *);

//int DST_check_structure(struct DynamicStructureTable *, struct StructureType *);

void DST_print(struct DynamicStructureTable *);

void DST_clear(void);

void DST_Global(void);

void DST_Local(void);


/*
struct SymbolsTableNumber *STNUM_insert(struct SymbolsTableNumber **, struct Constant);

struct SymbolsTableNumber *STNUM_find(struct SymbolsTableNumber **, struct Constant);

void SymbolsTableNumber_clear(struct SymbolsTableNumber **);

int STNUM_Test(struct SymbolsTableNumber **);
*/

extern struct SymbolsTable		*ST[HashingPrime];
extern struct ScopeType			Scope;

extern struct SymbolsTable		*SST[HashingPrime];
extern struct ScopeType			StructScope;

extern struct DynamicStructureTable		*DST;
extern int	DST_Scope;

extern struct DeclaredFunctionList		*DFL;

#endif
