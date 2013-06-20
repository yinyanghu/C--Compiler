struct Number
{
	int value, index;
};

int qsort(struct Number a[10], int s, int t)
{
	int i = s, j = t, x = a[i].value, y = a[i].index;
	while (i < j)
	{
		while (i < j && a[j].value > x) j = j - 1;
		if (i < j)
		{
			a[i].value = a[j].value;
			a[i].index = a[j].index;
			i = i + 1;
		}
		while (i < j && a[i].value < x) i = i + 1;
		if (i < j)
		{
			a[j].value = a[i].value;
			a[j].index = a[i].index;
			j = j - 1;
		}
	}
	a[i].value = x;
	a[i].index = y;
	if (s < i - 1) qsort(a, s, i-1);
	if (t > i + 1) qsort(a, i+1, t);
	return 0;
}
int main()
{
	int k = 0;
	struct Number b[10];
	while (k < 10)
	{
		b[k].value = -(10-k) * (10-k) + 20 * (10-k) - 50;
		b[k].index = k;
		k = k + 1;
	}
	k = qsort(b,0,9);
	k = 0;
	while (k < 10)
	{
		write(b[k].value);
		write(b[k].index);
		k = k + 1;
	}
	return 0;
}


