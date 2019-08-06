#include <iostream>
#include <cmath>
#include "gaus_func.h"

gaus_func::gaus_func(variable & x, variable & area, variable & mean, variable & sigma):
	function_base(1, {area.name(), mean.name(), sigma.name()})
{
}

gaus_func::~gaus_func()
{
}

double gaus_func::evaluate(const std::vector<double> & point) const
{
	if (point.size() != 1) {
		std::cout << "[gaus_func]: invalid data dimension (" << point.size() << "), required 1" << std::endl;
		return 0;
	}

	double area = get_par(0).value();
	double mean = get_par(1).value();
	double sigma = get_par(2).value();
	double x = point[0];
	
	return area * exp(-(x-mean)*(x-mean)/2/sigma/sigma) / sqrt(2*3.1415927) / sigma;
}
