int fibonacci(int n)
{
	int a = 0, b = 1, i = 0;
	while (i < n)
	{
		int c = a + b;
		write(b);
		a = b;
		b = c;
	}
}
