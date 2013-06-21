#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 3000;
const int Range = 13940;

int main()
{
	srand(time(NULL));
	cout << N << endl;		
	for (int i = 0; i < N; ++ i)
		cout << rand() % Range << endl;
	return 0;
}
