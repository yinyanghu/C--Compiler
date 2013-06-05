int f(int x)
{
	int i;
	if (x == 5) return 0;
	i = x;
	write(i);
	f(x + 1);
	write(i);
	return 0;
}

int main()
{
	f(1);
	return 0;
}
