int main()
{
	int A[10];
	int max, min;
	int i;

	// Generate
	i = 0;
	while (i < 10)
	{
		A[i] = 2 * i * i - 3 * i + 5;
		i = i + 1;
	}

	// Find Maximum and Minimum
	max = -10000000;
	min = 10000000;
	i = 0;
	while (i < 10)
	{
		if (A[i] < min)
			min = A[i];
		if (A[i] > max)
			max = A[i];
		i = i + 1;
	}
	write(min);
	write(max);
	return 0;
}
