#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

void df01_fit()
{
	TFile * f1 = TFile::Open("test-data/flat.root");
	TFile * f2 = TFile::Open("test-data/gaus.root");
	TFile * f3 = TFile::Open("test-data/bw.root");
	TTree * t_flat = (TTree *)f1->Get("t");
	TTree * t_gaus = (TTree *)f2->Get("t");
	TTree * t_bw   = (TTree *)f3->Get("t");

	dataset data_norm(t_flat, {"x"});
	dataset data_gaus(t_gaus, {"x"});
	dataset data_bw(t_bw, {"x"});
	
	cout << "********************* gaussian ********************" << endl;
	variable m("m", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m, s, data_norm);
	gaus.fit(data_gaus);
	
	TCanvas * c1 = new TCanvas("c1", "", 800, 800);
	c1->cd();
	TH1F * h1a = new TH1F("h1a", "", 100, -10, 10);
	TH1F * h1b = new TH1F("h1b", "", 100, -10, 10);
	data_gaus.draw(h1a);
	gaus.draw(h1b, h1a);
	
	
	cout << "********************* breit wigner *******************" << endl;
	variable w("w", 4, 0.3, 20);
	breitwigner bw(m, w, data_norm);
	bw.fit(data_bw, true);
	
	TCanvas * c2 = new TCanvas("c2", "", 800, 800);
	c2->cd();
	TH1F * h2a = new TH1F("h2a", "", 100, -10, 10);
	TH1F * h2b = new TH1F("h2b", "", 100, -10, 10);
	data_bw.draw(h2a);
	bw.draw(h2b, h2a);

	cout << "********************* integral ********************" << endl;
	variable m2("m2", 1);
	variable s2("s2", 2);
	gaussian gaus2(m2, s2, data_norm);
	cout << "integral of gaussian on [mean-1.0*sigma, mean+1.0*sigma]: " << gaus2.integral(m2.value()-s2.value(), m2.value()+s2.value()) << endl;	
	cout << "integral of gaussian on [mean-2.0*sigma, mean+2.0*sigma]: " << gaus2.integral(m2.value()-2*s2.value(), m2.value()+2*s2.value()) << endl;	
	cout << "integral of gaussian on [mean-3.0*sigma, mean+3.0*sigma]: " << gaus2.integral(m2.value()-3*s2.value(), m2.value()+3*s2.value()) << endl;	
	
	variable w2("w2", 2);
	breitwigner bw2(m2, w2, data_norm);
	cout << "integral of bw on [mean-0.5*width, mean+0.5*width]: " << bw2.integral(m2.value()-0.5*w2.value(), m2.value()+0.5*w2.value()) << endl;	
	cout << "integral of bw on [mean-1.0*width, mean+1.0*width]: " << bw2.integral(m2.value()-1.0*w2.value(), m2.value()+1.0*w2.value()) << endl;	
	cout << "integral of bw on [mean-2.0*width, mean+2.0*width]: " << bw2.integral(m2.value()-2.0*w2.value(), m2.value()+2.0*w2.value()) << endl;	
}
