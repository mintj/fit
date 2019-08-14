#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;
using namespace RooFit;

void zz()
{
	TFile * f2 = TFile::Open("test-data/mix.root");
	TTree * t_mix  = (TTree *)f2->Get("t");

	TH1F * h1 = new TH1F("h1", "", 40, -10, 10);
	t_mix->Draw("x>>h1", "");
	
	RooRealVar x("x", "", -10, 10);
	RooDataHist data("data", "", x, h1);

	RooRealVar m("m", "", 1, -10, 10);
	RooRealVar s("s", "", 4, 0.3, 20);
	RooGaussian gaus("gaus", "", x, m, s);
	
	RooRealVar m2("m2", "", 1, -10, 10);
	RooRealVar w("w", "", 4, 0.3, 20);
	RooBreitWigner bw("bw", "", x, m2, w);

	double ntot = data.sumEntries();
	RooRealVar ngaus("ngaus", "", 0.5*ntot, 0, ntot);
	RooRealVar nbw("nbw", "", 0.5*ntot, 0, ntot);
	RooAddPdf sum("sum", "", RooArgList(gaus, bw), RooArgList(ngaus, nbw));

	RooFitResult * fitr = sum.fitTo(data, Save());

	RooPlot * frame = x.frame();
	data.plotOn(frame);
	sum.plotOn(frame, LineColor(2));
	sum.plotOn(frame, Components("gaus"), LineColor(4), LineStyle(2));
	sum.plotOn(frame, Components("bw"), LineColor(3), LineStyle(2));
	frame->Draw();

	fitr->Print();
}
