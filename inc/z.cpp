class a
{
	public:
		void aha(int n) { cout << "(a): " << n << endl; }
		void yaha(int n) { cout << "a^2: " << n*n << endl; }
		template<typename... T> void aha(int n, T... r) { cout << n << " "; aha(r...); };
		template<typename... T> void yaha(int n, T... r) { cout << n*n << " "; yaha(r...); }
};

class b
{
	public:
		void aha(int n) { cout << "(b): " << n << endl; }
		template<typename... T> void aha(int n, T... r) { cout << "(deleted function)" << endl; };
		template<typename... T> void yaha(int n, T... r) { cout << "(deleted function)" << endl; }
};

void z()
{
	a x;
	x.aha(5);
	x.aha(1, 2, 3, 4, 5);
	x.yaha(5);
	x.yaha(1, 2, 3, 4, 5);
	
	b y;
	y.aha(5);
	y.aha(1, 2, 3, 4, 5);
	y.yaha(5);
	y.yaha(1, 2, 3, 4, 5);
}
