int main()
{
	int weight[21];
	int value[21];
	int f[21][101];
	int i, j;

	// Generate
	i = 1;
	while (i < 21)
	{
		weight[i] = 2 * i * i - 10 * i + 15;
		value[i] = i - (i / 8) * 8;
		i = i + 1;
	}

	// Initialize
	i = 0;
	while (i < 101)
	{
		f[0][i] = 0;
		i = i + 1;
	}

	// Dynamic Programming
	i = 1;
	while (i < 21)
	{
		j = 0;
		while (j < 101)
		{
			f[i][j] = f[i - 1][j];
			if (j >= weight[i] && f[i - 1][j - weight[i]] + value[i] > f[i][j])
				f[i][j] = f[i - 1][j - weight[i]] + value[i];
			j = j + 1;
		}
		i = i + 1;
	}

	write(f[20][100]);
	return 0;
}
