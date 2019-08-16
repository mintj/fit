void print(double a)
{
	cout << a << endl;
}

template <typename T>
void print(double a, const T & act)
{
	act(a);
	print(a);
}

template <typename T, typename... T2>
void print(double a, const T & act, const T2 & ... rest)
{
	act(a);
	print(a, rest...);
}

void sq(double & a)
{
	a *= a;
}

template <typename... T>
void bprint(double a, const T & ... act)
{
	if (sizeof...(act)) print(a, act...);
	else print(a);
}

void z()
{
	bprint(2);
	bprint(2, sq, sq);
}
