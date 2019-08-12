#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "../header.h"

using namespace std;

class gaussian2d: public pdf
{
	public:
		gaussian2d(variable & m1, variable & s1, variable & m2, variable & s2, variable & rho, dataset * normset = 0);
		virtual ~gaussian2d() {}
		virtual double evaluate(const double * x);
};

gaussian2d::gaussian2d(variable & m1, variable & s1, variable & m2, variable & s2, variable & rho, dataset * normset):
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

void demo7_2dfit()
{
	TFile * f = TFile::Open("DATA/weighted_2d.root");
	TTree * t = (TTree *)f->Get("t");

	dataset data_norm(t, {"x", "y"});
	dataset data_gaus(t, {"x", "y"}, "w");
	
	variable m1("m1", 1, -10, 10);
	variable s1("s1", 4, 0.1, 20);
	variable m2("m2", 1, -10, 10);
	variable s2("s2", 4, 0.1, 20);
	variable rho("rho", 0, -0.999, 0.999);
	gaussian2d gaus2d(m1, s1, m2, s2, rho, &data_norm);
	gaus2d.fit(&data_gaus);
}
