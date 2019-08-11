#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "../header.cpp"

using namespace std;

void demo1_fit()
{
	TFile * f1 = TFile::Open("DATA/flat.root");
	TFile * f2 = TFile::Open("DATA/gaus.root");
	TFile * f3 = TFile::Open("DATA/bw.root");
	TTree * t_flat = (TTree *)f1->Get("t");
	TTree * t_gaus = (TTree *)f2->Get("t");
	TTree * t_bw   = (TTree *)f3->Get("t");

	dataset data_norm(t_flat, {"x"});
	dataset data_gaus(t_gaus, {"x"});
	dataset data_bw(t_bw, {"x"});
	
	cout << "********************* gaussian ********************" << endl;
	variable m("m", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m, s, &data_norm);
	gaus.fit(&data_gaus);
	
	cout << "********************* breit wigner *******************" << endl;
	variable w("w", 4, 0.3, 20);
	breitwigner bw(m, w, &data_norm);
	bw.fit(&data_bw, true);
}
