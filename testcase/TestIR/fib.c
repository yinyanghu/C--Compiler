int f(int x)
{
	if (x == 0 || x == 1) return 1;
	return f(x - 1) + f(x - 2);
}

int main()
{
	int n;
	int ans;
	n = 25;
	ans = f(n);
	write(ans);	
	return 0;
}
