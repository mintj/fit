#include <iostream>
#include <cmath>
#include "dataset.h"
#include "gaussian.h" 

gaussian::gaussian(variable & m, variable & s, dataset * normset):
	pdf(1, {&m, &s}, normset)
{
}

gaussian::~gaussian()
{
}

double gaussian::evaluate(const double * x)
{
	double t = x[0];
	double m = get_par_ext(0);
	double s = get_par_ext(1);
	return exp(-(t-m)*(t-m)/2/s/s);
}
