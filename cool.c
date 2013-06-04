int f(int x)
{
	int i;
	if (x == 5) return 0;
	y = y + 1;
	i = x;
	write(i);
	f(x + 1);
	write(i);
	return 0;
}

int main()
{
	int y = 0;
	f(1);
	write(y);
	return 0;
}
