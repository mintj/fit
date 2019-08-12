#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "../header.h"

using namespace std;

void demo6_weightedfit()
{
	TFile * f = TFile::Open("DATA/weighted.root");
	TTree * t = (TTree *)f->Get("t");

	dataset data_norm(t, {"x"});
	dataset data_bw(t, {"x"}, "w1");
	dataset data_gaus(t, {"x"}, "w2");
	dataset data_mix(t, {"x"}, "w3");
	
	cout << "********************* breit wigner *******************" << endl;
	variable m1("m1", 1, -10, 10);
	variable w("w", 4, 0.01, 20);
	breitwigner bw(m1, w, &data_norm);
	//bw.fit(&data_bw, true);

	cout << "********************* gaussian ********************" << endl;
	variable m2("m2", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m2, s, &data_norm);
	//gaus.fit(&data_gaus);

	cout << "********************* ext fit ********************" << endl;
	double ntot = data_mix.nevt();
	variable n_bw("n_bw", 0.5*ntot, 0, ntot);
	variable n_gaus("n_gaus", 0.5*ntot, 0, ntot);
	//extpdf sum(&bw, &n_bw, &data_norm);
	extpdf sum(&gaus, &n_gaus, &data_norm);
	sum.fit(&data_gaus);
	//extpdf sum({&bw, &gaus}, {&n_bw, &n_gaus}, &data_norm);
	//sum.fit(&data_mix);
}
