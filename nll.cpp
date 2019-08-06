#include "nll.h"

nll::nll(pdf_base & p, data & d):
	m_pdf(p),
	m_data(d)
{
}

nll::~nll()
{
}

double nll::operator()(const std::vector<double> & par) const
{
	//m_pdf.read_from_pars(par);
	//double sum = -m_pdf.log_sum_on(m_data);
	//return sum;
	return 0;
}
