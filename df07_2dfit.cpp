#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "inc/header.h"

using namespace std;

class gaussian2d: public pdf
{
	public:
		gaussian2d(variable & m1, variable & s1, variable & m2, variable & s2, variable & rho, dataset & normset);
		virtual ~gaussian2d() {}
		virtual double evaluate(const double * x);
};

gaussian2d::gaussian2d(variable & m1, variable & s1, variable & m2, variable & s2, variable & rho, dataset & normset):
	pdf(2, {&m1, &s1, &m2, &s2, &rho}, normset)
{
}

double gaussian2d::evaluate(const double * x)
{
	double mx = get_par(0);
	double sx = get_par(1);
	double my = get_par(2);
	double sy = get_par(3);
	double rho = get_par(4);
	double tx = (x[0]-mx)/sx;
	double ty = (x[1]-my)/sy;
	return exp(-(tx*tx-2*rho*tx*ty+ty*ty)/2/(1-rho*rho));
}

void df07_2dfit()
{
	TFile * f = TFile::Open("test-data/weighted_2d.root");
	TTree * t = (TTree *)f->Get("t");

	dataset data_norm(t, {"x", "y"});
	dataset data_gaus(t, {"x", "y"}, "w");
	
	variable m1("m1", 1, -10, 10);
	variable s1("s1", 4, 0.1, 20);
	variable m2("m2", 1, -10, 10);
	variable s2("s2", 4, 0.1, 20);
	variable rho("rho", 0, -0.999, 0.999);
	gaussian2d gaus2d(m1, s1, m2, s2, rho, data_norm);
	gaus2d.fit(data_gaus);
	
	std::cout << "***********************************************" << std::endl;
	double x1 = m1.value()-s1.value();
	double x2 = m1.value()+s1.value();
	double y1 = m2.value()-s2.value();
	double y2 = m2.value()+s2.value();

	double ntot = 0, xtot = 0, ytot = 0;
	double x, y, w;
	t->SetBranchAddress("x", &x);
	t->SetBranchAddress("y", &y);
	t->SetBranchAddress("w", &w);
	for (int u = 0; u < t->GetEntries(); ++u) {
		t->GetEntry(u);
		ntot += w;
		if (x>x1 && x<x2) xtot += w;
		if (y>y1 && y<y2) ytot += w;
	}
	printf("integral of gaus2d on x(%f, %f) = %f (true value = %f)\n", x1, x2, gaus2d.integral(x1, x2, 0), xtot/ntot);
	printf("integral of gaus2d on y(%f, %f) = %f (true value = %f)\n", y1, y2, gaus2d.integral(y1, y2, 1), ytot/ntot);
}
