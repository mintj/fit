TRandom3 rndm;
double xlo = -10;
double xhi = 10;
double ylo = -10;
double yhi = 10;

bool evt_sel(double x, double y)
{
	double r2 = x*x + y*y;
	if (r2 > 70) return 0;
	else return (rndm.Rndm() > 0.2*r2/70);
}

double gaus2d(double x, double y, double mx, double sx, double my, double sy, double rho)
{
	double tx = (x-mx)/sx;
	double ty = (y-my)/sy;
	return exp(-(tx*tx-2*rho*tx*ty+ty*ty)/2/(1-rho*rho));
}

void gen_multid()
{
	TFile * f = TFile::Open("weighted_2d.root", "recreate");
	TTree * t = new TTree("t", "");
   	double x, y, w;
	t->Branch("x", &x, "x/D");
	t->Branch("y", &y, "y/D");
	t->Branch("w", &w, "w/D");
	for (int u = 0; u < 10000; ++u) {
		do {
			x = rndm.Uniform(xlo, xhi);
			y = rndm.Uniform(ylo, yhi);
		} while(!evt_sel(x, y));
		w = gaus2d(x, y, 1, 3.5, -4.5, 1, -0.5);
		t->Fill();
	}
	t->Write();
	f->Close();
}
