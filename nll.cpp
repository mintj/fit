#include <cmath>
#include "dataset.h"
#include "nll.h"
#include "pdf.h"

nll::nll(pdf * p, dataset * d):
	m_pdf(p),
	m_data(d)
{
}

nll::~nll()
{
}

double nll::operator()(const std::vector<double> & par) const
{
	double sum = m_pdf->log_sum(m_data, par);
	double norm = m_pdf->norm(par);
	return -sum-log(norm)*m_data->size();
}
