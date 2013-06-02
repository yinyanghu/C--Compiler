int gcd(int x, int y)
{
	if (y == 0)
		return x;
	else
		return gcd(y, x - y * (x / y));
}

int main()
{
	int a = read();
	int b = read();
	int c;
	c = gcd(a, b);
	write(c);
	return 0;
}
