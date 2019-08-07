#include <cmath>
#include "chi2.h"

chi2::chi2(function_base & f, dataset & d):
	m_func(f),
	m_data(d)
{
}

chi2::~chi2()
{
}

double chi2::operator()(const std::vector<double> & par) const
{
	m_func.read_from_pars(par);
	double sum = 0;
	for (size_t u = 0; u < m_data.size(); ++u) {
		double y0 = m_data.weight_at(u);
		double yf = m_func.value(*m_data.at(u));
		double err = m_data.err_at(u);
		if (err) sum += pow((yf-y0)/err, 2);
	}
	return sum;
}
