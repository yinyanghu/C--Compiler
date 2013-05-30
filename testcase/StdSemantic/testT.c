struct Complex
{
	float real, image;
};

float getReal(struct Complex x);

float getReal(struct Complex x)
{
	return x.real;
}

int main()
{
	float y;
	struct Complex c1;
	y = getReal(c1);
}
