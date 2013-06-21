int main()
{
	int A[7];	
	int i, j, k;
	A[0] = 2;
	A[1] = 7;
	A[2] = 3;
	A[3] = 1;
	A[4] = 5;
	A[5] = 6;
	A[6] = 4;
	i = 0;
	while (i < 7)
	{
		j = i + 1;
		while (j < 7)
		{
			if (A[i] < A[j])
			{
				k = A[i];
				A[i] = A[j];
				A[j] = k;
			}
			j = j + 1;
		}
		i = i + 1;
	}
	i = 0;
	while (i < 7)
	{
		write(A[i]);
		i = i + 1;
	}
	return 0;
}
