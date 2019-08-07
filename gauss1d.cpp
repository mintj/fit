#include <cmath>
#include "dataset.h"
#include "gauss1d.h" 
#include "variable.h"

gauss1d::gauss1d(variable & m, variable & s, dataset * normset):
	pdf(1, {&m, &s}, normset)
{
	m_var[0] = &m;
	m_var[1] = &s;
}

gauss1d::~gauss1d()
{
}

double gauss1d::evaluate(const double * x, const std::vector<double> & par)
{
	double t = x[0];
	double m = par[0];
	double s = par[1];
	return exp(-(t-m)*(t-m)/2/s/s);
}
