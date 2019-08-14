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
		virtual double func_weight(double x);
};

bw_proj::bw_proj(variable & m, variable & w, dataset & normset, size_t projdim, size_t nbin, double lo, double hi):
	projpdf({&m, &w}, normset, projdim, nbin, lo, hi)
{
}

bw_proj::bw_proj(variable & m, variable & w, dataset & normset, size_t projdim, size_t nbin, const double * binning):
	projpdf({&m, &w}, normset, projdim, nbin, binning)
{
}

double bw_proj::func_weight(double x)
{
	double m = get_par(0);
	double w = get_par(1);
	return 1.0/((x-m)*(x-m)+0.25*w*w);
}

class gaus_proj: public projpdf
{
	public:
		gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, double lo, double hi);
		gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, const double * binning);
		virtual ~gaus_proj() {}
		virtual double func_weight(double x);
};

gaus_proj::gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, double lo, double hi):
	projpdf({&m, &s}, normset, projdim, nbin, lo, hi)
{
}

gaus_proj::gaus_proj(variable & m, variable & s, dataset & normset, size_t projdim, size_t nbin, const double * binning):
	projpdf({&m, &s}, normset, projdim, nbin, binning)
{
}

double gaus_proj::func_weight(double x)
{
	double m = get_par(0);
	double s = get_par(1);
	return exp(-(x-m)*(x-m)/2/s/s);
}

void df09_projpdf2()
{
	TFile * f = TFile::Open("test-data/weighted_2d.root");
	TTree * t = (TTree *)f->Get("t");

	double ybinning[16] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 2, 4, 6, 8, 10};
	TH1F * h1 = new TH1F("h1", "", 20, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 15, ybinning);
	t->Draw("x>>h1", "w3", "goff");
	t->Draw("y>>h2", "w3", "goff");
	TCanvas * c = new TCanvas("c", "", 1600, 800);
	c->Divide(2, 1);
	c->cd(1);
	h1->Draw();
	c->cd(2);
	h2->Draw();
	
	dataset data_2d_norm(t, {"x", "y"});
	datahist data_x(h1);
	datahist data_y(h2);

	variable m("m", 1, -10, 10);
	variable w("w", 4, 0.1, 20);
	bw_proj bw_x(m, w, data_2d_norm, 0, 20, -10, 10);
	bw_proj bw_y(m, w, data_2d_norm, 1, 15, ybinning);

	std::cout << "***************************** fit on x ****************************" << std::endl;
	bw_x.fit(data_x, true);
	//std::cout << "***************************** fit on y ****************************" << std::endl;
	//bw_y.chi2fit();
	std::cout << "***************************** simfit on x+y ****************************" << std::endl;
	simfit comb;
	comb.add(bw_x, data_x);
	comb.add(bw_y, data_y);
	//comb.chi2fit(true);
}
