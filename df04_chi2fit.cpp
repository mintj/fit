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
	gaussian gaus(m1, s, data_norm);
	
	variable m2("m2", 1, -10, 10);
	variable w("w", 4, 0.3, 20);
	breitwigner bw(m2, w, data_norm);
	
	variable frac("frac", 0.5, 0, 1);
	addpdf sum({&gaus, &bw}, {&frac});
	
	cout << "********************* uniform binning ********************" << endl;
	sum.chi2fit(data_mix);
	
	TCanvas * c1 = new TCanvas("c1", "", 800, 800);
	c1->cd();
	TH1F * h3 = (TH1F *)h1->Clone("h3");
	h3->SetLineColor(2);
	h1->SetLineColor(1);
	data_mix.draw(h1);
	sum.draw(h3, h1);
	h1->Draw("e");
	h3->Draw("hist same");

	cout << "********************* coarse binning ********************" << endl;
	sum.chi2fit(data_mix2);
	
	TCanvas * c2 = new TCanvas("c2", "", 800, 800);
	c2->cd();
	TH1F * h4 = (TH1F *)h2->Clone("h4");
	h4->Reset();
	h4->SetLineColor(2);
	h2->SetLineColor(1);
	data_mix2.draw(h2);
	sum.draw(h4, h2);
	h2->Draw("e");
	h4->Draw("hist same");
}
