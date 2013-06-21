int gcd(int x, int y)
{
	if (y == 0)
		return x;
	else
		return gcd(y, x - y * (x / y));
}

int main()
{
	int a = 640;
	int b = 240;
	int c;
	c = gcd(a, b);
	write(c);
	return 0;
}
