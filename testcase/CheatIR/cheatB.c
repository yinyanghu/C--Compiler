int main()
{
	int commission, value;
	value = read();
	if (value < 2500)
		commission = 30 + value * 17 / 1000;
	else if (value < 6250 && value >= 2500)
		commission = 56 + value * 66 / 10000;
	else if (value < 20000)
		commission = 76 + value * 34 / 10000;
	else
		commission = 100 + value * 22 / 10000;
	if (commission < 39) commission = 39;
	if (commission < value && value / 5)
		write(commission);
	return 0;
}
