struct Complex
{
	float real, image;
};

float getImage(struct Complex c1);

float getReal(struct Complex c2);

int getImage(struct Complex c1);

float getReal(struct Complex c2, float k)
{
	k = k + c2.real;
	return k;
}

int main()
{
}
