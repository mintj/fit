#include <cmath>
#include "dataset.h"
#include "gauss1d.h" 

double fknorm(double m, double s)
{
	double t1 = (10-m)/s;
	double t2 = (m+10)/2;
	double a = 0.5*(TMath::Erf(t1/sqrt(2)) + TMath::Erf(t2/sqrt(2)));
	a *= sqrt(2*TMath::Pi())*s;
	return 20/a;
}

gauss1d::gauss1d(variable & m, variable & s, dataset * normset):
	pdf(1, {&m, &s}, normset)
{
}

gauss1d::~gauss1d()
{
}

double gauss1d::evaluate(const double * x)
{
	double t = x[0];
	double m = get_par_ext(0);
	double s = get_par_ext(1);
	return exp(-(t-m)*(t-m)/2/s/s);
}
