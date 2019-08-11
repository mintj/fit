#include <cmath>
#include "dataset.h"
#include "breitwigner.h" 

breitwigner::breitwigner(variable & m, variable & w, dataset * normset):
	pdf(1, {&m, &w}, normset)
{
}

breitwigner::~breitwigner()
{
}

double breitwigner::evaluate(const double * x)
{
	double t = x[0];
	double m = get_par_ext(0);
	double w = get_par_ext(1);
	return 1.0/((t-m)*(t-m)+0.25*w*w);
}
