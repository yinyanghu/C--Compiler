struct Complex
{float real, image;
};
struct abs
{
	int f3;
};
int abs3(int y)
{
	if (y >= 0) return y;
	else return -y;
}
float abs2(struct Complex x)
{
	return x.real * x.real + x.image * x.image;
}
struct abs
{
	int f1, f2;
};
int main()
{
	int i, j, a[10][10];
	struct Complex k, a;
	i = 0;
	if (i <= 10) j = abs3(i);
	while (i < 10)
	{
		j = j - 2;
		i = i + 1;
	}
	i+1;
}
