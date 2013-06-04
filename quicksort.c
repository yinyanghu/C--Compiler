int quicksort(int A[10], int l, int r)
{
	int i, j, x;
	if (l >= r) return 0;

	i = l; j = r; x = A[i];
	while (i < j)
	{
		while (i < j && A[j] < x) j = j - 1;
		if (i < j)
		{
			A[i] = A[j];
			i = i + 1;
		}
		while(i < j && A[i] > x) i = i + 1;
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
	int X[10];
	X[0] = 8;
	X[1] = 1;
	X[2] = 3;
	X[3] = 4;
	X[4] = 7;
	X[5] = 5;
	X[6] = 2;
	X[7] = 6;
	quicksort(X, 0, 7);
	return 0;
}

