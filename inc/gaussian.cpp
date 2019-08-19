#include <iostream>
#include <cmath>
#include "TMath.h"
#include "gaussian.h" 
#include "variable.h"

gaussian::gaussian(const char * name, variable & x, variable & m, variable & s):
	abspdf(name, {x.name()}, {m.name(), s.name()})
{
}

gaussian::~gaussian()
{
}

double gaussian::evaluate(const double * x)
{
	double t = x[0];
	double m = get_para(0);
	double s = get_para(1);
	return exp(-(t-m)*(t-m)/2/s/s);
}

double gaussian::norm()
{
	double m = get_para(0);
	double s = fabs(get_para(1));
	double xmin = min(0);
	double xmax = max(0);
	
	double s1 = (xmax-m)/s/sqrt(2);
	double s2 = (xmin-m)/s/sqrt(2);

	double area = 0.5 * (TMath::Erf(s1) - TMath::Erf(s2));
	return 1.0/sqrt(2*TMath::Pi())/s/area;
}
