#include "inc/header.h"

void df02_chi2fit()
{
	variable x("x", -10, 10);
	TH1F * h = new TH1F("h", "", 100, -10, 10);
	double m0 = -3;
	double s0 = 4.5;
	TRandom3 rndm;
	for (int u = 0; u < 100000; ++u) {
		double rx = rndm.Gaus(m0, s0);
		h->Fill(rx);
	}
	
	datahist dh("dh", x, h);

	variable m("m", 0, -10, 10);
	variable s("s", 2, 0.1, 10);
	gaussian model("model", x, m, s);

	model.chi2fit(dh, true);
	
	plot * frame = x.frame();
	dh.plot_on(frame);
	model.plot_on(frame, msfit::linecolor(2));
	frame->draw();
}
