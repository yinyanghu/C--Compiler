struct Complex
{
	float real, image;
};
float abs2(struct Complex x);
int abs(float x);
float abs2(int x);
float abs2(struct Complex x)
{
	struct Complex2 z;
	return x.real * x.real + x.image * x.image;
}
int abs(int y)
{
	if (y >= 0) return y;
	else return -y;
}int main()
{
	int i, j, a[10][10];
	i = 0;
	if (i <= 10) j = abs();
	while (i < 10)
	{
		j = j - 2;
		i = i + 1;
	}
}
