struct Complex
{
	float real, image, i;
};
int abs(int x)
{
	if (x >= 0) return x;
	else return -x;
}
float abs2(struct Complex x)
{
	return x.real * x.real + x.image * x.image;
}
int write(int i)
{
	return 0;
}
int main()
{
	int i, j, a[10][10];
	struct Complex k;
	i = 0;
	if (i <= 10) j = abs(i);while (i < 10)
	{
		float j;
		j = i + 2;
		if (j >= 5.0)
		{
			int k = abs(j);
			k.real - 3;
		}
	}
	write(i);
}
