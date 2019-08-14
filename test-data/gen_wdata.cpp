TRandom3 rndm;
double lo = -10;
double hi = 10;

double int_bw(double m, double w)
{
	double dm1 = hi-m;
	double dm2 = lo-m;
	double a = 2/w;
	return a*(atan(a*dm1)-atan(a*dm2));
}

double int_gaus(double m, double s)
{
	double s1 = (hi-m)/s;
	double s2 = (lo-m)/s;
	return 0.5*(TMath::Erf(s1)-TMath::Erf(s2));
}

bool evt_sel(double x)
{
	double eff = 0.6 + 0.4*(x-lo)/(hi-lo);
	if (x<lo || x>hi) eff = 0;
	else eff = 1;
	return (rndm.Rndm() < eff);
}

double bw(double x, double m, double w)
{
	return 1.0/((x-m)*(x-m)+0.25*w*w);
}

double gaus(double x, double m, double s)
{
	return exp(-(x-m)*(x-m)/2/s/s)/sqrt(2*TMath::Pi())/s;
}

double gen_flat()
{
	double x = rndm.Uniform(lo, hi);
	while (!evt_sel(x)) {
		x = rndm.Uniform(lo, hi);
	}
	return x;
}

void write_data(const char * name, int nevt, const vector<double> & par)
{
	TFile * f = TFile::Open(name, "recreate");
	TTree * t = new TTree("t", "");
	double x, w1, w2, w3;
	t->Branch("x", &x, "x/D");
	t->Branch("w1", &w1, "w1/D");
	t->Branch("w2", &w2, "w2/D");
	t->Branch("w3", &w3, "w3/D");
	double int1 = int_bw(par[0], par[1]);
	double int2 = int_gaus(par[2], par[3]);
	double frac = par[4]*int2/((1-par[4])*int1+par[4]*int2);
	cout << par[4] << " " << frac << endl;
	for (int u = 0; u < nevt; ++u) {
		x = gen_flat();
		w1 = bw(x, par[0], par[1]);
		w2 = gaus(x, par[2], par[3]);
		w3 = frac*w1 + (1-frac)*w2;
		t->Fill();
	}
	t->Write();
	f->Close();
}

void gen_wdata()
{
	write_data("weighted.root", 100000, {-4.1, 0.2, 5.5, 6.5, 0.33});
}
