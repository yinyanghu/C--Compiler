struct Complex
{
	float real, image;
};
struct Complex multiply(struct Complex x, struct Complex y)
{
	struct Complex z;
	z.real = x.real * y.real - x.image * y.image;
	z.image = x.real * y.image + y.real * x.image;
	return z;
}
struct Complex divide(struct Complex x, struct Complex y)
{
	struct Complex z;
	float abs_sqr = y.real * y.real + y.image * y.image;
	z.real = (x.real * y.real + x.image * y.image) / abs_sqr;
	z.image = (y.real * x.image - x.real * y.image) / abs_sqr;
	return z;
}
