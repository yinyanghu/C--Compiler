#ifndef IR_H
#define IR_H

extern struct IRChain		*IR;

int	get_label(void);
int get_var(struct VariableAttribute *);
struct Operand	*ID_Operand(char *, int);

struct Operand	*Build_Operand(OperandType, int);
struct IRCode	*Build_IRCode(IRCodeType, struct Operand *, struct Operand *, struct Operand *, BinaryOP_Relop, int, char *);


struct IRChain *IRChain_build(struct IRCode *);
struct IRChain *IRChain_link(struct IRChain *, struct IRChain *);
void Print_IRChain(struct IRChain *);



void GeneratingIR(struct Program *);
struct IRChain *IR_Program(struct Program *);
struct IRChain *IR_ExtDefList(struct ExtDefList *);
struct IRChain *IR_ExtDef(struct ExtDef *);

struct IRChain *IR_ExtDef_A(void *);
struct IRChain *IR_ExtDef_B(void *);
struct IRChain *IR_ExtDef_C(void *);
struct IRChain *IR_ExtDef_D(void *);

struct IRChain *IR_FunDec(struct FunDec *);
struct IRChain *IR_CompSt(struct CompSt *);
struct IRChain *IR_DefList(struct DefList *);
struct IRChain *IR_StmtList(struct StmtList *);

struct IRChain *IR_Stmt(struct Stmt *);
struct IRChain *IR_Stmt_Exp(void *);
struct IRChain *IR_Stmt_Return(void *);
struct IRChain *IR_Stmt_CompSt(void *);
struct IRChain *IR_Stmt_If(void *);
struct IRChain *IR_Stmt_If_Else(void *);
struct IRChain *IR_Stmt_While(void *);

struct IRChain *IR_Def(struct Def *);
struct IRChain *IR_DecList(struct DecList *);
struct IRChain *IR_Dec(struct Dec *);
struct IRChain *IR_VarDec(struct VarDec *, struct Operand *);
struct IRChain *IR_VarDec_A(void *, struct Operand *);
struct IRChain *IR_VarDec_B(void *, struct Operand *);


struct IRChain *IR_Condition(struct Exp *, int, int);
struct IRChain *IR_Condition_RELOP(void *, int, int);
struct IRChain *IR_Condition_NOT(void *, int, int);
struct IRChain *IR_Condition_AND(void *, int, int);
struct IRChain *IR_Condition_OR(void *, int, int);
struct IRChain *IR_Condition_Par(void *, int, int);


struct IRChain *IR_Exp(struct Exp *, struct Operand *, int);
struct IRChain *IR_Exp_INT(void *, struct Operand *, struct TYPE *, int);
struct IRChain *IR_Exp_ID(void *, struct Operand *, struct TYPE *, int);
struct IRChain *IR_Exp_Assign(void *, struct Operand *, struct TYPE *, int);
struct IRChain *IR_Exp_Minus(void *, struct Operand *, struct TYPE *, int);
struct IRChain *IR_Exp_ADD_SUB_MUL_DIV(void *, struct Operand *, struct TYPE *, int);
struct IRChain *IR_Exp_Par(void *, struct Operand *, struct TYPE *, int);
struct IRChain *IR_Exp_Attribute(void *, struct Operand *, struct TYPE *, int);
struct IRChain *IR_Exp_Array(void *, struct Operand *, struct TYPE *, int);

struct IRChain *IR_Exp_Function(void *, struct Operand *, struct TYPE *, int);


struct IRChain *IR_Args(struct Args *, struct ArgsChain **);

#endif
