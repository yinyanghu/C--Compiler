int wtf(int a, int b, int c, int d, int e, int f, int g, int h) {
	int value=a-(a/2)*2;
	int temp[8];
	if (a>=10&&value==0) {
		temp[0]=a;
		temp[1]=b+a;
		temp[2]=c+2*a;
		temp[3]=d+3*a;
		temp[4]=e+4*a;
		temp[5]=f-2*a;
		temp[6]=g-3*a;
		temp[7]=h-4*a;
	}
	else {
		int array[8];
		array[0]=value;
		array[1]=b;
		array[2]=c;
		array[3]=d;
		array[4]=e;
		array[5]=f;
		array[6]=g;
		array[7]=h;
		return array[0]*array[1]+array[2]* array[3]+
			array[4]*array[5]+array[6]*array[7];
	}
	{
		int i = 0;
		int result =wtf(a-1, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-2, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-3, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-4, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-5, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-6, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-7, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-8, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-9, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]) +
			wtf(a-10, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
		while (i<8) {
			if (i == 0)
				result = result+a;
			else if (i == 1)
				result = result+b;
			else if (i == 2)
				result = result+c;
			else if (i == 3)
				result = result+d;
			else if (i == 4)
				result = result+e;
			else if (i == 5)
				result = result+f;
			else if (i == 6)
				result = result+g;
			else
				result = result+h;
			result = result + temp[i];
			i = i + 1;
		}
		return result;
	}
}

int main() {
	int n=read();
	int r=wtf(n,n,n,n,n,n,n,n);
	write(r);
	return 0;
}
