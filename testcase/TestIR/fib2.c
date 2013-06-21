int main()
{
	int a, b, c;
	int n;
	int i;
	n = 40;
	a = 1; b = 1;
	i = 2;
	while (i <= n)
	{
		c = a + b;
		if (i >= 35)
			write(c);
		a = b; b = c;
		i = i + 1;
	}
	return 0;
}
