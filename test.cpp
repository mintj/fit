#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "header.cpp"

using namespace std;

//int main(int argc, char ** argv)
void test()
{
	variable x("x");
	dataset data_norm(x);
	dataset data_test(x);
	TRandom3 rndm;
	for (int u = 0; u < 1000000; ++u) {
		data_norm.add({rndm.Uniform(-10, 10)});
	}
	for (int u = 0; u < 1000; ++u) {
		data_test.add({rndm.Gaus(0.4, 2.2)});
	}

	double ntot = 100;
	variable n("n", ntot, 0, 100*ntot);
	variable m("m", 0, -5, 5);
	variable s("s", 1, 0.1, 5);

	gaus_func ajaja(x, n, m, s);
	chi2 fcn(ajaja, data_test);

	//gaus_pdf ahaha(x, m, s, data_norm);
	//nll fcn(ahaha, data_test);
	
	mn_helper mn(fcn);
	mn.minimize(true);
}
