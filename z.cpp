#include <iostream>
#include <iomanip>
#include "TMath.h"
#include "TTree.h"
#include "TRandom3.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
//#include "header.cpp"

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
	//return 1;
	double eff = 0.8+0.02*x;
	if (x<-10 || x>10) eff = 0;
	else eff = 1;
	return (rndm.Rndm() < eff);
}

TTree * gen_uniform(const char * name, int ntot, double lo, double hi)
{
	TTree * t = new TTree(name, "");
	double _xx;
	t->Branch("x", &_xx, "x/D");
	TRandom3 rndm;
	for (int u = 0; u < ntot; ++u) {
		_xx = rndm.Uniform(lo, hi);
		if (evt_sel(_xx, rndm)) t->Fill();
	}
	return t;
}

TTree * gen_gauss(const char * name, int ntot, double m, double s)
{
	TTree * t = new TTree(name, "");
	double _xx;
	t->Branch("x", &_xx, "x/D");
	TRandom3 rndm;
	for (int u = 0; u < ntot; ++u) {
		_xx = rndm.Gaus(m, s);
		if (evt_sel(_xx, rndm)) t->Fill();
	}
	return t;
}

void write_data(double m, double s)
{
	TFile * f = TFile::Open("a-test-data.root", "recreate");
	TTree * t = gen_gauss("t", 10000, m, s);
	t->Write();
	f->Close();
}

TTree * get_tree()
{
	TFile * f = TFile::Open("a-test-data.root");
	TTree * t = (TTree *)f->Get("t");
	return t;
}

void z()
{
	write_data(1.5, 8.5);
	TTree * t = get_tree();

	RooRealVar x("x", "", -10, 10);
	RooDataSet data("data", "", t, x);

	RooRealVar m("m", "", 1, -10, 10);
	RooRealVar s("s", "", 1, 0.1, 10);

	RooGaussian gaus("gaus", "", x, m, s);
	RooFitResult * fitr = gaus.fitTo(data, RooFit::Save());

	RooPlot * frame = x.frame();
	data.plotOn(frame);
	gaus.plotOn(frame);
	frame->Draw();

	fitr->Print();
}

int main()
{
	z();
}
