#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "../header.h"

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
	
	cout << "********************* integral ********************" << endl;
	variable m2("m2", 1);
	variable s2("s2", 2);
	gaussian gaus2(m2, s2, &data_norm);
	cout << "integral of gaussian on [mean-1*sigma, mean+1*sigma]: " << gaus2.integral(m2.value()-s2.value(), m2.value()+s2.value()) << endl;	
	cout << "integral of gaussian on [mean-2*sigma, mean+2*sigma]: " << gaus2.integral(m2.value()-2*s2.value(), m2.value()+2*s2.value()) << endl;	
	cout << "integral of gaussian on [mean-3*sigma, mean+3*sigma]: " << gaus2.integral(m2.value()-3*s2.value(), m2.value()+3*s2.value()) << endl;	
	
	variable w2("w2", 2);
	breitwigner bw2(m2, w2, &data_norm);
	cout << "integral of bw on [mean-1*width, mean+1*width]: " << bw2.integral(m2.value()-w2.value(), m2.value()+w2.value()) << endl;	
	cout << "integral of bw on [mean-2*width, mean+2*width]: " << bw2.integral(m2.value()-2*w2.value(), m2.value()+2*w2.value()) << endl;	
	cout << "integral of bw on [mean-3*width, mean+3*width]: " << bw2.integral(m2.value()-3*w2.value(), m2.value()+3*w2.value()) << endl;	
}
