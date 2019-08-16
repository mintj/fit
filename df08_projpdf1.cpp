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

	double ybinning[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20};
	TH1F * h1 = new TH1F("h1", "", 20, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 15, ybinning);
	t->Draw("x>>h1", "w2", "goff");
	t->Draw("y>>h2", "w2", "goff");
	
	dataset data_2d_norm(t, {"x", "y"});
	datahist data_x(h1);
	datahist data_y(h2);

	variable m1("m1", 1, -10, 10);
	variable w("w", 1, 0.1, 20);
	bw_proj bw_x(m1, w, data_2d_norm, 0, 20, -10, 10);
	bw_x.fit(data_x);
	
	variable m2("m2", 10, 0, 20);
	variable s("s", 5, 0.1, 20);
	gaus_proj gaus_y(m2, s, data_2d_norm, 1, 15, ybinning);
	gaus_y.fit(data_y);

	TCanvas * c = new TCanvas("c", "", 1600, 800);
	c->Divide(2, 1);

	c->cd(1);
	plot * frame1 = new plot;
	data_x.plot_on(frame1);
	bw_x.plot_on(frame1, msfit::linecolor(2));
	frame1->draw();

	c->cd(2);
	plot * frame2 = new plot;
	data_y.plot_on(frame2);
	gaus_y.plot_on(frame2, msfit::linecolor(2));
	frame2->draw();
}
