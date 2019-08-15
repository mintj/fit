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

void df09_projpdf2()
{
	TFile * f = TFile::Open("test-data/weighted_2d.root");
	TTree * t = (TTree *)f->Get("t");

	double ybinning[16] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 2, 4, 6, 8, 10};
	TH1F * h1 = new TH1F("h1", "", 20, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 15, ybinning);
	t->Draw("x>>h1", "w3", "goff");
	t->Draw("y>>h2", "w3", "goff");
	
	dataset data_2d_norm(t, {"x", "y"});
	datahist data_x(h1);
	datahist data_y(h2);

	variable m("m", 1, -10, 10);
	variable w("w", 4, 0.1, 20);
	bw_proj bw_x(m, w, data_2d_norm, 0, 20, -10, 10);
	bw_proj bw_y(m, w, data_2d_norm, 1, 15, ybinning);

	std::cout << "***************************** fit on x ****************************" << std::endl;
	bw_x.fit(data_x, true);
	
	TCanvas * c1 = new TCanvas("c1", "", 800, 800);
	c1->cd();
	TH1F * h1a = new TH1F("h1a", "", 100, -10, 10);
	TH1F * h1b = new TH1F("h1b", "", 100, -10, 10);
	data_x.draw(h1a);
	bw_x.draw(h1b, h1a);
	h1b->SetLineColor(2);
	h1b->Draw("hist same");
	
	std::cout << "***************************** fit on y ****************************" << std::endl;
	bw_y.chi2fit(data_y);
	
	TCanvas * c2 = new TCanvas("c2", "", 800, 800);
	c2->cd();
	TH1F * h2a = new TH1F("h2a", "", 100, -10, 10);
	TH1F * h2b = new TH1F("h2b", "", 100, -10, 10);
	data_y.draw(h2a);
	bw_y.draw(h2b, h2a);
	h2b->SetLineColor(2);
	h2b->Draw("hist same");
	
	std::cout << "***************************** simfit on x+y ****************************" << std::endl;
	simfit comb;
	comb.add(bw_x, data_x);
	comb.add(bw_y, data_y);
	comb.chi2fit(true);
	
	TCanvas * c3 = new TCanvas("c3", "", 1600, 800);
	c3->Divide(2, 1);
	TH1F * h3a = new TH1F("h3a", "", 100, -10, 10);
	TH1F * h3b = new TH1F("h3b", "", 100, -10, 10);
	TH1F * h4a = new TH1F("h4a", "", 100, -10, 10);
	TH1F * h4b = new TH1F("h4b", "", 100, -10, 10);
	
	c3->cd(1);
	data_x.draw(h3a);
	bw_x.draw(h3b, h3a);
	h3b->SetLineColor(2);
	h3b->Draw("hist same");
	
	c3->cd(2);
	data_y.draw(h4a);
	bw_y.draw(h4b, h4a);
	h4b->SetLineColor(2);
	h4b->Draw("hist same");
}
