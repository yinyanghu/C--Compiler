struct CC {
	int C;
	int D;
};

struct AA {
	struct BB {
		struct CC	A[2];
		struct CC	B[2];
	} x[2];
	
};

int f(int x)
{
	if (x == 0 || x == 1) return 1;
	return x * f(x - 1);
}

int main()
{
	struct AA	P;
	int i = 0;
	P.x[0].A[0].C = 1;
	P.x[0].A[0].D = 2;
	P.x[0].A[1].C = 3;
	P.x[0].A[1].D = 4;
	P.x[0].B[0].C = 5;
	P.x[0].B[0].D = 6;
	P.x[0].B[1].C = 7;
	P.x[0].B[1].D = 8;

	P.x[1].A[0].C = 1;
	P.x[1].A[0].D = 2;
	P.x[1].A[1].C = 3;
	P.x[1].A[1].D = 4;
	P.x[1].B[0].C = 5;
	P.x[1].B[0].D = 6;
	P.x[1].B[1].C = 7;
	P.x[1].B[1].D = 8;
	/*
	while (i < 2)
	{
		P.x[i].A = f(i + 4);
		P.x[i].B = f(i + 5);
		P.y[i].C = f(i + 1);
		P.y[i].D = f(i + 2);
		i = i + 1;
	}
	*/
	/*
	P.x[0].A = 1;
	P.x[0].B = 2;
	P.y.C = -2;
	P.y.D = -1;
	*/
	return 0;
}
