int main()
{
	int digits = 0, n, counter = 0, m, remain;
	n = read();
	while (n > 0)
	{
		remain = n - (n/10) * 10;
		n = n / 10;
		m = digits = digits + 1;
		while (m > 0 && remain > 0)
		{
			counter = counter + m * remain;
			m = m - 1;
			remain = remain - 1;
		}
	}
	write(digits);
	write(counter);
	return 0;
}
