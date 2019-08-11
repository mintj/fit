#include <iostream>
#include <cmath>
#include "datahist.h"
#include "chi2.h"
#include "pdf.h"

chi2::chi2(pdf * p, dataset * d):
	m_pdf(p),
	m_data(d)
{
}

chi2::~chi2()
{
}

double chi2::operator()(const std::vector<double> & par) const
{
	//double sum = m_pdf->log_sum(m_data, par);
	//double norm = m_pdf->norm(par);
	//return -sum-log(norm)*m_data->size();
}
