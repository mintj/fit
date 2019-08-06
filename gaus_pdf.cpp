#include <iostream>
#include <cmath>
#include "gaus_pdf.h"

gaus_pdf::gaus_pdf(variable & x, variable & mean, variable & sigma, data & data_norm):
	pdf_base(1, {mean.name(), sigma.name()}, data_norm)
{
	m_status = normalize();
	std::cout << "norm = " << m_norm << std::endl;
}

gaus_pdf::~gaus_pdf()
{
}

double gaus_pdf::evaluate(const std::vector<double> & point) const
{
	if (point.size() != 1) {
		std::cout << "[gaus_pdf]: invalid data dimension (" << point.size() << "), required 1" << std::endl;
		return 0;
	}

	double mean = get_par(0).value();
	double sigma = get_par(1).value();
	double x = point[0];

	//std::cout << get_par(0).name() << " " << get_par(1).name() << std::endl;
	//std::cout << x << " " << mean << " " << sigma << " " << exp(-(x-mean)*(x-mean)/2/sigma/sigma) << std::endl;	
	return exp(-(x-mean)*(x-mean)/2/sigma/sigma);
}
