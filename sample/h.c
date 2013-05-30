struct Complex
{
	float real, image
};

struct Complex multiply(struct Complex x: struct Complex y)
{
	struct Complex z;
	z.real = (x.real*y.real - x.image* y.image;
	z.image = x.real*y.image + y.real*x.image;
	return z;
}

int main()
{
	struct Complex a, b, c;
	if (a.real > b.real)
		multiply(a, c)
	else
		multiply((b, c);
	return 0;
}
