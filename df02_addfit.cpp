#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

void df02_addfit()
{
	TFile * f1 = TFile::Open("test-data/flat.root");
	TFile * f2 = TFile::Open("test-data/mix.root");
	TTree * t_flat = (TTree *)f1->Get("t");
	TTree * t_mix  = (TTree *)f2->Get("t");

	dataset data_norm(t_flat, {"x"});
	dataset data_mix(t_mix, {"x"});
	
	variable m1("m1", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m1, s, data_norm);
	
	variable m2("m2", 1, -10, 10);
	variable w("w", 4, 0.3, 20);
	breitwigner bw(m2, w, data_norm);
	
	variable frac("frac", 0.5, 0, 1);
	addpdf sum({&gaus, &bw}, {&frac});
	sum.fit(data_mix, true);

	TH1F * h1 = new TH1F("h1", "", 100, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 100, -10, 10);
	TH1F * h3 = new TH1F("h3", "", 100, -10, 10);
	TH1F * h4 = new TH1F("h4", "", 100, -10, 10);
	data_mix.draw(h1);
	sum.draw(h2, h1);
	sum.draw_comp(h3, 0, h1);
	sum.draw_comp(h4, 1, h1);
	h1->SetLineColor(1);
	h2->SetLineColor(2);
	h3->SetLineColor(3);
	h4->SetLineColor(4);
	h1->Draw("e");
	h2->Draw("hist same");
	h3->Draw("hist same");
	h4->Draw("hist same");

	std::cout << "\n****************************************" << std::endl;
	std::cout << "integral of gaus on (1, 2) = " << gaus.integral(1, 2) << std::endl;
	std::cout << "integral of bw on (1, 2) = " << bw.integral(1, 2) << std::endl;
	std::cout << "integral of sum on (1, 2) = " << sum.integral(1, 2) << std::endl;
}
