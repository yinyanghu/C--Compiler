#ifndef OPTIMIZE_H
#define OPTIMIZE_H

void IRChain_Delete(struct IRChain *);

void IRChain_Insert(struct IRChain *, struct IRChain *);

struct IRChain	*IR_Optimizer(struct IRChain *, FILE *);

int Checking_Operand(struct Operand *, struct Operand *);
int Checking_Constant(struct Operand *);

#endif
