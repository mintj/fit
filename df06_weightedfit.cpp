#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

void df06_weightedfit()
{
	TFile * f = TFile::Open("test-data/weighted.root");
	TTree * t = (TTree *)f->Get("t");

	dataset data_norm(t, {"x"});
	dataset data_bw(t, {"x"}, "w1");
	dataset data_gaus(t, {"x"}, "w2");
	dataset data_mix(t, {"x"}, "w3");
	
	cout << "********************* breit wigner *******************" << endl;
	variable m1("m1", 1, -10, 10);
	variable w("w", 4, 0.01, 20);
	breitwigner bw(m1, w, data_norm);
	bw.fit(data_bw, true);

	cout << "********************* gaussian ********************" << endl;
	variable m2("m2", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m2, s, data_norm);
	gaus.fit(data_gaus);

	cout << "********************* add fit ********************" << endl;
	variable frac("frac", 0.5, 0, 1);
	addpdf sum({&bw, &gaus}, {&frac});
	sum.fit(data_mix, true);
}
