#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

void df06_weightedfit()
{
	TFile * f = TFile::Open("test-data/weighted.root");
	TTree * t = (TTree *)f->Get("t");

	dataset data_norm(t, {"x"});
	dataset data_bw(t, {"x"}, "w1");
	dataset data_gaus(t, {"x"}, "w2");
	dataset data_mix(t, {"x"}, "w3");
	
	cout << "********************* breit wigner *******************" << endl;
	variable m1("m1", 1, -10, 10);
	variable w("w", 4, 0.01, 20);
	breitwigner bw(m1, w, data_norm);
	bw.fit(data_bw, true);

	TCanvas * c1 = new TCanvas("c1");
	c1->cd();
	plot * frame1 = new plot;
	data_bw.plot_on(frame1);
	bw.plot_on(frame1, msfit::linecolor(2));
	frame1->draw();

	cout << "********************* gaussian ********************" << endl;
	variable m2("m2", 1, -10, 10);
	variable s("s", 4, 0.3, 20);
	gaussian gaus(m2, s, data_norm);
	gaus.fit(data_gaus);

	TCanvas * c2 = new TCanvas("c2");
	c2->cd();
	plot * frame2 = new plot;
	data_gaus.plot_on(frame2);
	gaus.plot_on(frame2, msfit::linecolor(2));
	frame2->draw();

	cout << "********************* add fit ********************" << endl;
	variable frac("frac", 0.5, 0, 1);
	addpdf sum({&bw, &gaus}, {&frac});
	sum.fit(data_mix);

	TCanvas * c3 = new TCanvas("c3");
	c3->cd();
	plot * frame3 = new plot;
	data_mix.plot_on(frame3);
	sum.plot_on(frame3, msfit::linecolor(2));
	sum.plot_on(frame3, msfit::components(0), msfit::linecolor(3), msfit::linestyle(2));
	sum.plot_on(frame3, msfit::components(1), msfit::linecolor(4), msfit::linestyle(2), msfit::name("wori"));
	frame3->draw();
}
