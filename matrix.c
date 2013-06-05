int MatrixMultiplication(int A[3][2], int B[2][4], int C[3][4])
{
	int k = 0;
	int i = 0, j = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 4)
		{
			k = 0;
			C[i][j] = 0;
			while (k < 2)
			{
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
				k = k + 1;
			}
			j = j + 1;
		}
		i = i + 1;
	}
	return -1;
}

int main()
{
	int X[3][2];
	int Y[2][4];
	int Z[3][4];
	int a, b;
	X[0][0] = 1;
	X[0][1] = -1;
	X[1][0] = 2;
	X[1][1] = 0;
	X[2][0] = 3;
	X[2][1] = 1;

	Y[0][0] = 1;
	Y[0][1] = 1;
	Y[0][2] = 1;
	Y[0][3] = 1;
	Y[1][0] = -1;
	Y[1][1] = 2;
	Y[1][2] = -2;
	Y[1][3] = 1;

	write(MatrixMultiplication(X, Y, Z));
	a = 0;
	while (a < 3)
	{
		b = 0;
		while (b < 4)
		{
			write(Z[a][b]);
			b = b + 1;
		}
		a = a + 1;
	}
	return 0;
}
