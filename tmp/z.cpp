#include <cstdio>

const int * p(int * a, int n)
{
	return a+n;
}

int main()
{
	int * a;
	int b[3][4] = {{1, 2, 3, 4},
	               {5, 6, 7, 8},
	               {9, 10, 11, 12}};
	a = b[0];
	auto l = p(a, 1);
	printf("%d\n", *l);
}
