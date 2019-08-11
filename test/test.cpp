#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "../header.cpp"

using namespace std;

TRandom3 rndm;
double lo = -10;
double hi = 10;

double cnorm(double m, double s)
{
	double t1 = (10-m)/s;
	double t2 = (m+10)/2;
	double a = 0.5*(TMath::Erf(t1/sqrt(2)) + TMath::Erf(t2/sqrt(2)));
	a *= sqrt(2*TMath::Pi())*s;
	return 20/a;
}

bool evt_sel(double x)
{
	//return 1;
	double eff = 0.6 + 0.4*(x-lo)/(hi-lo);
	if (x<lo || x>hi) eff = 0;
	else eff = 1;
	return (rndm.Rndm() < eff);
}

double gen_flat(const vector<double> & par)
{
	double x = rndm.Uniform(lo, hi);
	while (!evt_sel(x)) {
		x = rndm.Uniform(lo, hi);
	}
	return x;
}

double gen_gaus(const vector<double> & par)
{
	double x = rndm.Gaus(par[0], par[1]);
	while (!evt_sel(x)) {
		x = rndm.Gaus(par[0], par[1]);
	}
	return x;
}

double gen_bw(const vector<double> & par)
{
	double x = rndm.BreitWigner(par[0], par[1]);
	while (!evt_sel(x)) {
		x = rndm.BreitWigner(par[0], par[1]);
	}
	return x;
}

double gen_mix(const vector<double> & par)
{
	double f = par[0];
	if (rndm.Rndm() < f) {
		vector<double> p = {par[1], par[2]};
		return gen_gaus(p);
	}
	else {
		vector<double> p = {par[3], par[4]};
		return gen_bw(p);
	}
}

void write_data(const char * name, int nevt, function<double(const vector<double> &)> generator, const vector<double> & par)
{
	TFile * f = TFile::Open(name, "recreate");
	TTree * t = new TTree("t", "");
	double _xx;
	t->Branch("x", &_xx, "x/D");
	for (int u = 0; u < nevt; ++u) {
		_xx = generator(par);
		t->Fill();
	}
	t->Write();
	f->Close();
}

TTree * get_tree(const char * name)
{
	TFile * f = TFile::Open(name);
	TTree * t = (TTree *)f->Get("t");
	return t;
}

void test()
{
	write_data("DATA/flat.root", 100000, gen_flat, {});
	write_data("DATA/gaus.root", 10000, gen_gaus, {1.5, 4.5});
	write_data("DATA/bw.root", 10000, gen_bw, {2.5, 3.5});
	write_data("DATA/mix.root", 10000, gen_mix, {0.3, 0.3, 6.2, 2.5, 1.5});
	TTree * t_flat = get_tree("DATA/flat.root");
	TTree * t_gaus = get_tree("DATA/gaus.root");
	TTree * t_bw   = get_tree("DATA/bw.root");
	TTree * t_mix  = get_tree("DATA/mix.root");
	TH1F * h1 = new TH1F("h1", "", 200, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 200, -10, 10);
	TH1F * h3 = new TH1F("h3", "", 200, -10, 10);
	t_gaus->Draw("x>>h1");
	t_bw->Draw("x>>h2");
	t_mix->Draw("x>>h3");
	h1->Draw();
	h2->Draw("Same");
	h3->Draw("Same");
	//cout << h1->Integral() << " " << h2->Integral() << endl;

	dataset data_norm(t_flat, {"x"});
	dataset data_gaus(t_gaus, {"x"});
	dataset data_bw(t_bw, {"x"});
	dataset data_mix(t_mix, {"x"});
	
	variable m1("m1", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m1, s, &data_norm);
	
	variable m2("m2", 1, -10, 10);
	variable w("w", 4, 0.3, 20);
	breitwigner bw(m2, w, &data_norm);
	
	//nll fcn(&gaus, &data_gaus);
	//nll fcn(&bw, &data_bw);

	double ntot = data_gaus.size();
	variable ngaus("ngaus", 0.5*ntot, 0, ntot);
	variable nbw("nbw", 0.5*ntot, 0, ntot);
	extpdf sum({&gaus, &bw}, {&ngaus, &nbw}, &data_norm);
	nll fcn(&sum, &data_mix);

	mn_helper mn(fcn);
	mn.minimize(true);
}
