int abs(int z)
{
	if (z >= 0) return z;
	else return -z;
}
float abs2(int x, int y)
{
	if (x-y >= 0) return 1.0;
	else return -1.0;
}
int main()
{
	int i, j, a[10][10];
	float k;
	i = 0;
	if (i <= 10) j = abs(i);
	while (i < 10)
	{
		k = k + 1.0;
		a[i][k] = j;
	}
	a[abs2(i,j)][3] = 9;
	j;
}
