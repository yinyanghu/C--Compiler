int main()
{
	int w[11], v[11], i, j = 1, maxW = 50;
	int f[11][51];
	i = 0;
	while (i < 10)
	{
		w[i+1] = i * i - 8 * i + 19;
		j = 7 * j + 2;
		while (j > 13) j = j - 13;
		v[i+1] = j;
		f[0][i+1] = 0;
		i = i + 1;
	}
	i = 1;
	while (i <= 10)
	{
		j = 0;
		while (j <= 50)
		{
			f[i][j] = f[i-1][j];
			if (j >= w[i]) if (f[i-1][j-w[i]]+v[i] > f[i][j]) f[i][j] = f[i-1][j-w[i]] + v[i];
			j = j + 1;
		}
		i = i + 1;
	}
	write(f[10][50]);
	return 0;
}
