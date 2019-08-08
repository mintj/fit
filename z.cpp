#include <iostream>
#include "TMath.h"

using namespace std;

class a
{
	public:
		a(double v) { mv = v; }

	protected:
		double val() { return mv; }

	protected:
		double mv;
};

class b: public a
{
	public:
		b(double v): a(v) {}
		void print(a & wtf) { cout << mv << " " << wtf.val() << endl; }
};

int main()
{
	//cout << TMath::Poisson(1000, 1000) << endl;
	a x(10);
	b z(5);
	z.print(x);
}
