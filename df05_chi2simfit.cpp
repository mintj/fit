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
	gaussian gaus(m, w, data_norm);
	breitwigner bw(m, w, data_norm);
	
	simfit comb;
	comb.add(gaus, data_gaus);
	comb.add(bw, data_bw);
	comb.chi2fit(true);

	TCanvas * c = new TCanvas("c", "", 1600, 800);
	c->Divide(2, 1);
	c->cd(1);
	TH1F * h3 = (TH1F *)h1->Clone("h3");
	gaus.draw(h3, h1);
	h1->SetLineColor(1);
	h3->SetLineColor(2);
	h1->Draw("e");
	h3->Draw("hist same");
	
	c->cd(2);
	TH1F * h4 = (TH1F *)h2->Clone("h4");
	bw.draw(h4, h2);
	h2->SetLineColor(1);
	h4->SetLineColor(2);
	h2->Draw("e");
	h4->Draw("hist same");
}
