struct Operands
{
	int o1;
	int o2;
};
int Add(struct Operands temp)
{
	return (temp.o1 + temp.o2);
}
int main()
{
	int n;
	struct Operands op;
	op.o1 = 1;
	op.o2 = 2;
	n = Add(op);
	write(n);
	return 0;
}
