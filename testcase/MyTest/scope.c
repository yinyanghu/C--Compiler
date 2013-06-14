int i;
int f()
{
	int i = 1;
	write(i);
	if (1)
	{
		int i = 2;
		write(i);
	}
	write(i);
	return 0;
}

int main()
{
	i = 3;
	write(i);
	f();
	write(i);
	return 0;
}
