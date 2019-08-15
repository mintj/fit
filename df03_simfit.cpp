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
	gaussian gaus(m, w, data_norm);
	breitwigner bw(m, w, data_norm);
	
	simfit comb;
	comb.add(gaus, data_gaus);
	comb.add(bw, data_bw);
	comb.fit(true);
	
	TCanvas * c = new TCanvas("c", "", 1600, 800);
	c->Divide(2, 1);
	c->cd(1);
	TH1F * h1 = new TH1F("h1", "", 100, -10, 10);
	TH1F * h3 = new TH1F("h3", "", 100, -10, 10);
	data_gaus.draw(h1);
	gaus.draw(h3, h1);
	h1->SetLineColor(1);
	h3->SetLineColor(2);
	h1->Draw("e");
	h3->Draw("hist same");
	
	c->cd(2);
	TH1F * h2 = new TH1F("h2", "", 100, -10, 10);
	TH1F * h4 = new TH1F("h4", "", 100, -10, 10);
	data_bw.draw(h2);
	bw.draw(h4, h2);
	h2->SetLineColor(1);
	h4->SetLineColor(2);
	h2->Draw("e");
	h4->Draw("hist same");
}
