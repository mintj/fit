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

	double ybinning[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20};
	TH1F * h1 = new TH1F("h1", "", 20, -10, 10);
	TH1F * h2 = new TH1F("h2", "", 15, ybinning);
	t->Draw("x>>h1", "w3", "goff");
	t->Draw("y>>h2", "w3", "goff");
	
	dataset data_2d_norm(t, {"x", "y"});
	datahist data_x(h1);
	datahist data_y(h2);

	variable m("m", 1, -10, 20);
	variable w("w", 4, 0.1, 20);
	bw_proj bw_x(m, w, data_2d_norm, 0, 20, -10, 10);
	bw_proj bw_y(m, w, data_2d_norm, 1, 15, ybinning);

	std::cout << "***************************** fit on x ****************************" << std::endl;
	bw_x.fit(data_x, true);
	
	TCanvas * c1 = new TCanvas("c1", "", 800, 800);
	c1->cd();
	plot * frame1 = new plot;
	data_x.plot_on(frame1);
	bw_x.plot_on(frame1, msfit::linecolor(2));
	frame1->draw();

	std::cout << "***************************** fit on y ****************************" << std::endl;
	bw_y.fit(data_y);
	
	TCanvas * c2 = new TCanvas("c2", "", 800, 800);
	c2->cd();
	plot * frame2 = new plot;
	data_y.plot_on(frame2);
	bw_y.plot_on(frame2, msfit::linecolor(2));
	frame2->draw();
	
	std::cout << "***************************** simfit on x+y ****************************" << std::endl;
	simfit comb;
	comb.add(bw_x, data_x);
	comb.add(bw_y, data_y);
	comb.fit();
	
	TCanvas * c3 = new TCanvas("c3", "", 1600, 800);
	c3->Divide(2, 1);
	
	c3->cd(1);
	plot * frame3 = new plot;
	data_x.plot_on(frame3);
	bw_x.plot_on(frame3, msfit::linecolor(2));
	frame3->draw();
	
	c3->cd(2);
	plot * frame4 = new plot;
	data_y.plot_on(frame4);
	bw_y.plot_on(frame4, msfit::linecolor(2));
	frame4->draw();
}
