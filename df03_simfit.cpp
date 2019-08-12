#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

void df03_simfit()
{
	TFile * f1 = TFile::Open("test-data/flat.root");
	TFile * f2 = TFile::Open("test-data/comb.root");
	TTree * t_flat = (TTree *)f1->Get("t");
	TTree * t_gaus = (TTree *)f2->Get("t1");
	TTree * t_bw   = (TTree *)f2->Get("t2");

	dataset data_norm(t_flat, {"x"});
	dataset data_gaus(t_gaus, {"x"});
	dataset data_bw(t_bw, {"x"});
	
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
	
	cout << "************************ comb1: pdf + pdf ***********************" << endl;
	simfit comb1({&gaus, &bw}, {&data_gaus, &data_bw});
	comb1.fit(true);

	cout << "************************ comb2: extpdf + pdf ***********************" << endl;
	simfit comb2({&gaus_ext, &bw}, {&data_gaus, &data_bw});
	comb2.fit(true);

	cout << "************************ comb1: pdf + extpdf ***********************" << endl;
	simfit comb3({&gaus, &bw_ext}, {&data_gaus, &data_bw});
	comb3.fit(true);

	cout << "************************ comb1: extpdf + extpdf ***********************" << endl;
	simfit comb4({&gaus_ext, &bw_ext}, {&data_gaus, &data_bw});
	comb4.fit(true);
}
