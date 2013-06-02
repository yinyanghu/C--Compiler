int main()
{
	int a, b, c;
	int n;
	int i;
	n = read();
	a = 1; b = 1;
	i = 2;
	while (i <= n)
	{
		c = a + b;
		a = b; b = c;
		i = i + 1;
	}
	write(c);
	return 0;
}
