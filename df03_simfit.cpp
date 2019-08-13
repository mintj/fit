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
	gaussian gaus(m, w, &data_norm);
	breitwigner bw(m, w, &data_norm);
	
	simfit comb;
	comb.add(&gaus, &data_gaus);
	comb.add(&bw, &data_bw);
	comb.fit(true);
}
