#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

void df04_chi2fit()
{
	TFile * f1 = TFile::Open("test-data/flat.root");
	TFile * f2 = TFile::Open("test-data/mix.root");
	TTree * t_flat = (TTree *)f1->Get("t");
	TTree * t_mix  = (TTree *)f2->Get("t");

	TH1F * h1 = new TH1F("h1", "", 40, -10, 10);
	t_mix->Draw("x>>h1", "");
	
	double binning[] = {-10, -8, -6, -5, -4, -3, -2, -1, 0, 0.5,
	                    1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6,
	                    7, 8, 9, 10};
	TH1F * h2 = new TH1F("h2", "", 23, binning);
	t_mix->Draw("x>>h2", "");

	dataset data_norm(t_flat, {"x"});
	datahist data_mix(h1);
	datahist data_mix2(h2);
	
	variable m1("m1", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m1, s);
	
	variable m2("m2", 1, -10, 10);
	variable w("w", 4, 0.3, 20);
	breitwigner bw(m2, w);
	
	double ntot = data_mix.nevt();
	variable n_gaus("n_gaus", 0.5*ntot, 0, ntot);
	variable n_bw("n_bw", 0.5*ntot, 0, ntot);
	extpdf sum({&gaus, &bw}, {&n_gaus, &n_bw}, &data_norm);
	cout << "********************* uniform binning ********************" << endl;
	sum.chi2fit(&data_mix);
	cout << "********************* coarse binning ********************" << endl;
	sum.chi2fit(&data_mix2);
}
