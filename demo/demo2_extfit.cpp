#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "../header.cpp"

using namespace std;

void demo2_extfit()
{
	TFile * f1 = TFile::Open("DATA/flat.root");
	TFile * f2 = TFile::Open("DATA/mix.root");
	TTree * t_flat = (TTree *)f1->Get("t");
	TTree * t_mix  = (TTree *)f2->Get("t");

	dataset data_norm(t_flat, {"x"});
	dataset data_mix(t_mix, {"x"});
	
	variable m1("m1", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m1, s);
	
	variable m2("m2", 1, -10, 10);
	variable w("w", 4, 0.3, 20);
	breitwigner bw(m2, w);
	
	double ntot = data_mix.size();
	variable n_gaus("n_gaus", 0.5*ntot, 0, ntot);
	variable n_bw("n_bw", 0.5*ntot, 0, ntot);
	extpdf sum({&gaus, &bw}, {&n_gaus, &n_bw}, &data_norm);
	sum.fit(&data_mix, true);
}
