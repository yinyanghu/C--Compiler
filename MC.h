#ifndef MC_H
#define MC_H

struct RegisterType {
	char	name[6];	
	//int		free;
	int		next_use;
	struct Operand	*key;
};

// Store Local Variable in Stack
struct StackFrame {
	int		offset;
	int		size;
	struct Operand	*key;
};

void MC_Prologue(FILE *);
void GeneratingMC(struct IRChain *);


void Stack_clear(void);
int Stack_push(struct Operand *, int);
int Stack_offset(struct Operand *);


char *Register_name(struct RegisterType *);
void Register_update(void);
void Register_clear(void);
void Register_free(struct RegisterType *);
void Register_spill(struct RegisterType *);
struct RegisterType *Register_allocate(struct Operand *);
struct RegisterType *Register_find(struct Operand *);
struct RegisterType *Register_get(struct Operand *);

void MC_SP(int);
void MC_Load(struct RegisterType *, int, struct RegisterType *);
void MC_Store(struct RegisterType *, int, struct RegisterType *);


#endif
