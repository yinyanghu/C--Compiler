struct Pair
{
	int value, index;
};
int sort(int n)
{
	struct Pair a[100];
	int i = 0, j = 0;
	while (i < n)
	{
		a[i].value = -i * i + 20 * i - 50;
		a[i].index = i;
		i = i + 1;
	}
	i = 0;
	while (i < n)
	{
		j = 0;
		while (j < i)
		{
			if (a[i].value < a[j].value)
			{
				struct Pair t;
				t.value = a[i].value;
				t.index = a[i].index;
				a[i].value = a[j].value;
				a[i].index = a[j].index;
				a[j].value = t.value;
				a[j].index = t.index;
			}
			j = j + 1;
		}
		i = i + 1;
	}
	i = 0;
	while (i < n)
	{
		write(a[i].value);
		write(a[i].index);
		i = i + 1;
	}
	return 0;
}

int main()
{
	int m;
	m = read();
	if (m >= 100) write(-1);
	else sort(m);
	return 0;
}
