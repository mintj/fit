TRandom3 rndm;
double lo = -10;
double hi = 10;

bool evt_sel(double x)
{
	double eff = 0.6 + 0.4*(x-lo)/(hi-lo);
	if (x<lo || x>hi) eff = 0;
	else eff = 1;
	return (rndm.Rndm() < eff);
}

double gen_flat(const vector<double> & par)
{
	double x = rndm.Uniform(lo, hi);
	while (!evt_sel(x)) {
		x = rndm.Uniform(lo, hi);
	}
	return x;
}

double gen_gaus(const vector<double> & par)
{
	double x = rndm.Gaus(par[0], par[1]);
	while (!evt_sel(x)) {
		x = rndm.Gaus(par[0], par[1]);
	}
	return x;
}

double gen_bw(const vector<double> & par)
{
	double x = rndm.BreitWigner(par[0], par[1]);
	while (!evt_sel(x)) {
		x = rndm.BreitWigner(par[0], par[1]);
	}
	return x;
}

double gen_mix(const vector<double> & par)
{
	double f = par[0];
	if (rndm.Rndm() < f) {
		vector<double> p = {par[1], par[2]};
		return gen_gaus(p);
	}
	else {
		vector<double> p = {par[3], par[4]};
		return gen_bw(p);
	}
}

void write_data(const char * name, int nevt, function<double(const vector<double> &)> generator, const vector<double> & par)
{
	TFile * f = TFile::Open(name, "recreate");
	TTree * t = new TTree("t", "");
	double x;
	t->Branch("x", &x, "x/D");
	for (int u = 0; u < nevt; ++u) {
		x = generator(par);
		t->Fill();
	}
	t->Write();
	f->Close();
}

TTree * get_tree(const char * name)
{
	TFile * f = TFile::Open(name);
	TTree * t = (TTree *)f->Get("t");
	return t;
}

void gen_data()
{
	write_data("flat.root", 100000, gen_flat, {});
	write_data("gaus.root", 10000, gen_gaus, {1.5, 4.5});
	write_data("bw.root", 10000, gen_bw, {2.5, 3.5});
	write_data("mix.root", 10000, gen_mix, {0.27, 0.3, 6.2, 2.5, 1.5});

	TFile * f = TFile::Open("comb.root", "recreate");
	TTree * t1 = new TTree("t1", "");
	TTree * t2 = new TTree("t2", "");
	double x;
	t1->Branch("x", &x, "x/D");
	t2->Branch("x", &x, "x/D");
	for (int u = 0; u < 8000; ++u) {
		x = gen_gaus({2.7, 4.5});
		t1->Fill();
	}
	for (int u = 0; u < 15000; ++u) {
		x = gen_bw({2.7, 4.5});
		t2->Fill();
	}
	t1->Write();
	t2->Write();
	f->Close();
}
