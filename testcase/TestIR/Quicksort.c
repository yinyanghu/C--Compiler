int quicksort(int A[3000], int l, int r)
{
	int i, j, x;
	if (l >= r) return 0;

	i = l; j = r; x = A[i];
	while (i < j)
	{
		while (i < j && A[j] > x) j = j - 1;
		if (i < j)
		{
			A[i] = A[j];
			i = i + 1;
		}
		while(i < j && A[i] < x) i = i + 1;
		if (i < j)
		{
			A[j] = A[i];
			j = j - 1;
		}
	}

	A[i] = x;

	quicksort(A, l, i - 1);
	quicksort(A, i + 1, r);
	return 0;
}

int main()
{
	int X[3000];
	int N;
	int i;
	N = read();
	i = 0;
	while (i < N)
	{
		X[i] = read();
		i = i + 1;
	}
	quicksort(X, 0, N - 1);
	i = 0;
	while (i < N)
	{
		write(X[i]);
		i = i + 1;
	}
	return 0;
}

