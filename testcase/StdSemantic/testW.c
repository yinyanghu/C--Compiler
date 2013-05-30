struct Complex
{
	float real,image;
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

int main()
{
	struct Complex x;
	float real, image, cabs;
	real = x.real;
	image = x.image;
	cabs = abs2(x);

}
