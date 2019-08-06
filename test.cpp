#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "header.h"

using namespace std;

int main(int argc, char ** argv)
{
	variable x("x");
	data data_norm(x);
	data data_test(x);
	TRandom3 rndm;
	for (int u = 0; u < 1000000; ++u) {
		data_norm.add({rndm.Uniform(-10, 10)});
	}
	for (int u = 0; u < 1000; ++u) {
		data_test.add({rndm.Gaus(atof(argv[1]), atof(argv[2]))});
	}
	
	variable m("m", 0);
	variable s("s", 1);
	
	gaus_pdf ahaha(x, m, s, data_norm);

	//nll fcn(gaus_pdf, data_test);
	//mn_helper mn(fcn);
	//mn.minimize();
}
