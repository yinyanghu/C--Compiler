int multiTest(int a1, int a2, int a3, int a4)
{
if (a1 > a2 && a2 > a3 && a3 > a4 || a4 > a1)
return 1;
else
return (a1 + a2 * (a3 / a4));
}
int f(int n)
{
if (n == 1) return multiTest(1, 2, 3, 4);
return f(n-1) + n * 2 + 1;
}
int f2(int m)
{
if (m == 1)
return multiTest(4,3,2,1);
else
return f(m-1) + 2 * f(m-2) - 1;
}
int main()
{
int k;
k = read();
if (k <= 0)
write(multiTest(2,1,3,1));
else
write(f2(k));
return 0;
}
