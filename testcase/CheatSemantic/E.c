int abs(int x)
{
	if (x >= 0) return x;
	else return -x;
}
int main()
{
	int i, j, a[10][10];
	float k = 3;
	while (i < 10)
	{
		j = k;
		k = k + 1.0;
	}
	k = abs(j);
}
