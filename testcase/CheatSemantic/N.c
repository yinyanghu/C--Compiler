struct Complex
{
	float real, image;
	struct Attribute
	{
		float magnitude;
	} attr;
};
int abs(int y)
{
	if (y >= 0) return y;
	else return -y;
}
float abs2(struct Complex x)
{
	if (x.real == x.magnitude) return 0.0;
	return x.attr.magnitude;
}
int main()
{
	int i, j, a[10][10];
	struct Complex k;
	i = 0;
	if (i <= 10) j = abs(i);
	while (i < 10)
	{
		j = j - 1;
		i = i + k.attr.image;
	}
	i;
}
