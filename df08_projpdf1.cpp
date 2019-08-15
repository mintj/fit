#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

class bw_proj: public projpdf
{
	public:
		bw_proj(variable & m, variable & w, dataset & normset, size_t projdim, size_t nbin, double lo, double hi);
		bw_proj(variable & m, variable & w, dataset & normset, size_t projdim, size_t nbin, const double * binning);
		virtual ~bw_proj() {}
		virtual double func_weight(const double * x);
};

bw_proj::bw_proj(variable & m, variable & w, dataset & normset, size_t projdim, size_t nbin, double lo, double hi):
	projpdf({&m, &w}, normset, projdim, nbin, lo, hi)
{
}

bw_proj::bw_proj(variable & m, variable & w, dataset & normset, size_t projdim, size_t nbin, const double * binning):
	projpdf({&m, &w}, normset, projdim, nbin, binning)
{
}

double bw_proj::func_weight(const double * x)
{
	double m = get_par(0);
	double w = get_par(1);
	return 1.0/((x[0]-m)*(x[0]-m)+0.25*w*w);
}

class gaus_proj: public projpdf
{
	public:
		gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, double lo, double hi);
		gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, const double * binning);
		virtual ~gaus_proj() {}
		virtual double func_weight(const double * x);
};

gaus_proj::gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, double lo, double hi):
	projpdf({&m, &s}, normset, projdim, nbin, lo, hi)
{
}

gaus_proj::gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, const double * binning):
	projpdf({&m, &s}, normset, projdim, nbin, binning)
{
}

double gaus_proj::func_weight(const double * x)
{
	double m = get_par(0);
	double s = get_par(1);
	return exp(-(x[1]-m)*(x[1]-m)/2/s/s);
}

void df08_projpdf1()
{
	TFile * f = TFile::Open("test-data/weighted_2d.root");
	TTree * t = (TTree *)f->Get("t");

	double ybinning[16] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 2, 4, 6, 8, 10};
	TH1F * h1 = new TH1F("h1", "", 20, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 15, ybinning);
	t->Draw("x>>h1", "w2", "goff");
	t->Draw("y>>h2", "w2", "goff");
	
	dataset data_2d_norm(t, {"x", "y"});
	datahist data_x(h1);
	datahist data_y(h2);

	variable m1("m1", 1, -10, 10);
	variable w("w", 4, 0.1, 20);
	bw_proj bw_x(m1, w, data_2d_norm, 0, 20, -10, 10);
	bw_x.chi2fit(data_x);
	
	variable m2("m2", 1, -10, 10);
	variable s("s", 4, 0.1, 20);
	gaus_proj gaus_y(m2, s, data_2d_norm, 1, 15, ybinning);
	gaus_y.chi2fit(data_y);

	TCanvas * c = new TCanvas("c", "", 1600, 800);
	c->Divide(2, 1);
	TH1F * h1a = new TH1F("h1a", "", 100, -10, 10);
	TH1F * h1b = new TH1F("h1b", "", 100, -10, 10);
	TH1F * h2a = new TH1F("h2a", "", 100, -10, 10);
	TH1F * h2b = new TH1F("h2b", "", 100, -10, 10);

	c->cd(1);
	data_x.draw(h1a);
	bw_x.draw(h1b, h1a);
	h1b->SetLineColor(2);
	h1a->Draw();
	h1b->Draw("hist same");

	c->cd(2);
	data_y.draw(h2a);
	gaus_y.draw(h2b, h2a);
	h2b->SetLineColor(2);
	h2a->Draw();
	h2b->Draw("hist same");
}
