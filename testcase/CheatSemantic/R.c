struct Complex
{
	float real, image;
};
int abs(int y);
float abs2(struct Complex x);
float abs2(struct Complex x)
{struct Complex z;
	return x.real * x.real + x.image * x.image;
}
int abs(int y);
int main()
{
	int i, j, a[10][10];
	struct Complex k;
	i = 0;
	if (i <= 10) j = abs(i);
	while (i < 10)
	{
		j = j - 2;
		i = i + 1;
	}
	i*6;
}
int main();
float abs2(struct Complex x);
