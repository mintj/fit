#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

void df05_chi2simfit()
{
	TFile * f1 = TFile::Open("test-data/flat.root");
	TFile * f2 = TFile::Open("test-data/comb.root");
	TTree * t_flat = (TTree *)f1->Get("t");
	TTree * t_gaus = (TTree *)f2->Get("t1");
	TTree * t_bw   = (TTree *)f2->Get("t2");

	dataset data_norm(t_flat, {"x"});
	
	TH1F * h1 = new TH1F("h1", "", 40, -10, 10);
	t_gaus->Draw("x>>h1", "");
	
	double binning[] = {-10, -8, -6, -5, -4, -3, -2, -1, 0, 0.5,
	                    1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6,
	                    7, 8, 9, 10};
	TH1F * h2 = new TH1F("h2", "", 23, binning);
	t_bw->Draw("x>>h2", "");
	
	datahist data_gaus(h1);
	datahist data_bw(h2);
	
	variable m("m", 1, -10, 10);
	variable w("w", 4, 0.3, 20);
	gaussian gaus(m, w);
	breitwigner bw(m, w);
	
	double n1 = data_gaus.nevt();
	double n2 = data_bw.nevt();
	variable n_gaus("n_gaus", n1, 0.5*n1, 1.5*n1);
	variable n_bw("n_bw", n2, 0.5*n2, 1.5*n2);
	extpdf gaus_ext(&gaus, &n_gaus, &data_norm);
	extpdf bw_ext(&bw, &n_bw, &data_norm);
	
	simfit comb({&gaus_ext, &bw_ext}, {&data_gaus, &data_bw});
	comb.chi2fit(true);
}
