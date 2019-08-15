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

	TCanvas * c1 = new TCanvas("c1");
	c1->cd();
	TH1F * h1a = new TH1F("h1a", "", 100, -10, 10);
	TH1F * h1b = new TH1F("h1b", "", 100, -10, 10);
	data_bw.draw(h1a);
	bw.draw(h1b, h1a);

	cout << "********************* gaussian ********************" << endl;
	variable m2("m2", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m2, s, data_norm);
	gaus.fit(data_gaus);

	TCanvas * c2 = new TCanvas("c2");
	c2->cd();
	TH1F * h2a = new TH1F("h2a", "", 100, -10, 10);
	TH1F * h2b = new TH1F("h2b", "", 100, -10, 10);
	data_gaus.draw(h2a);
	gaus.draw(h2b, h2a);
	h2b->SetLineColor(2);
	h2b->Draw("hist same");

	cout << "********************* add fit ********************" << endl;
	variable frac("frac", 0.5, 0, 1);
	addpdf sum({&bw, &gaus}, {&frac});
	sum.fit(data_mix, true);

	TCanvas * c3 = new TCanvas("c3");
	c3->cd();
	TH1F * h3a = new TH1F("h3a", "", 100, -10, 10);
	TH1F * h3b = new TH1F("h3b", "", 100, -10, 10);
	data_mix.draw(h3a);
	sum.draw(h3b, h3a);
	h3b->SetLineColor(2);
	h3a->Draw("e1");
	h3b->Draw("hist same");
}
