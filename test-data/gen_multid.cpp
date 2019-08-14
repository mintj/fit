TRandom3 rndm;
double xlo = -10;
double xhi = 10;
double ylo = -10;
double yhi = 10;

bool evt_sel(double x, double y)
{
	double r2 = x*x + y*y;
	if (r2 > 120) return 0;
	else return (rndm.Rndm() > 0.2*r2/120);
}

double gaus2d(double x, double y, double mx, double sx, double my, double sy, double rho)
{
	double tx = (x-mx)/sx;
	double ty = (y-my)/sy;
	return exp(-(tx*tx-2*rho*tx*ty+ty*ty)/2/(1-rho*rho));
}

double bw(double x, double m, double w)
{
	return 1.0/((x-m)*(x-m)+0.25*w*w);
}

double bw_gaus(double x, double y, double mx, double wx, double my, double sy)
{
	double b = 1.0/((x-mx)*(x-mx)+0.25*wx*wx);
	double g = exp(-(y-my)*(y-my)/2/sy/sy);
	return b*g;
}

void gen_multid()
{
	TFile * f = TFile::Open("weighted_2d.root", "recreate");
	TTree * t = new TTree("t", "");
   	double x, y, w1, w2, w3;
	t->Branch("x", &x, "x/D");
	t->Branch("y", &y, "y/D");
	t->Branch("w1", &w1, "w1/D");
	t->Branch("w2", &w2, "w2/D");
	t->Branch("w3", &w3, "w3/D");
	for (int u = 0; u < 10000; ++u) {
		do {
			x = rndm.Uniform(xlo, xhi);
			y = rndm.Uniform(ylo, yhi);
		} while(!evt_sel(x, y));
		w1 = gaus2d(x, y, 1, 3.5, -4.5, 1, -0.5);
		w2 = bw_gaus(x, y, 2.2, 3.5, -4.2, 1.9);
		w3 = bw(x, 4.4, 1.7);
		t->Fill();
	}
	t->Write();
	f->Close();
}
