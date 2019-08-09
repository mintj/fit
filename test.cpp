#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "header.cpp"

using namespace std;

double norm(double m, double s)
{
	double t1 = (10-m)/s;
	double t2 = (m+10)/2;
	double a = 0.5*(TMath::Erf(t1/sqrt(2)) + TMath::Erf(t2/sqrt(2)));
	a *= sqrt(2*TMath::Pi())*s;
	return 20/a;
}

bool evt_sel(double x, TRandom3 & rndm)
{
	return 1;
	//double eff = 0.8+0.02*x;
	//if (x<-10 || x>10) eff = 0;
	////else eff = 1;
	//return (rndm.Rndm() < eff);
}

TTree * gen_uniform(const char * name, int ntot, double lo, double hi)
{
	TTree * t = new TTree(name, "");
	double x;
	t->Branch("x", &x, "x/D");
	TRandom3 rndm;
	for (int u = 0; u < ntot; ++u) {
		x = rndm.Uniform(lo, hi);
		if (evt_sel(x, rndm)) t->Fill();
	}
	return t;
}

TTree * gen_gauss(const char * name, int ntot, double m, double s)
{
	TTree * t = new TTree(name, "");
	double x;
	t->Branch("x", &x, "x/D");
	TRandom3 rndm;
	for (int u = 0; u < ntot; ++u) {
		x = rndm.Gaus(m, s);
		if (evt_sel(x, rndm)) t->Fill();
	}
	return t;
}

void test()
{
	TTree * tree_norm = gen_uniform("t1", 100000, -10, 10);
	TTree * tree_gaus = gen_gauss("t2", 10000, 1.5, 3);
	TH1F * h1 = new TH1F("h1", "", 200, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 200, -10, 10);
	tree_norm->Draw("x>>h1");
	tree_gaus->Draw("x>>h2");
	h1->Draw();
	h2->Draw("Same");
	cout << h1->Integral() << " " << h2->Integral() << endl;

	dataset data_norm(tree_norm, {"x"});
	dataset data_gaus(tree_gaus, {"x"});
	
	variable m("m", 1, -10, 10);
	variable s("s", 4, 0.3, 20);

	gauss1d gaus(m, s, &data_norm);
	
	//nll fcn(&gaus, &data_gaus);
	//cout << gaus.norm() << " " << norm(m.value(), s.value()) << endl;

	double ntot = data_gaus.size();
	variable n("n", ntot, 0.5*ntot, 2*ntot);
	extpdf sum(&gaus, &n, &data_norm);
	nll fcn(&sum, &data_gaus);

	mn_helper mn(fcn);
	mn.minimize(true);
}
