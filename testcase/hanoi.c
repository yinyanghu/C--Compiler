int hanoi(int n, int a, int b, int c)
{
	if (n == 0) return 0;	
	hanoi(n - 1, a, c, b);
	write(a);
	write(c);
	hanoi(n - 1, b, a, c);
	return 0;
}

int main()
{
	hanoi(4, 1, 2, 3);
	return 0;
}
