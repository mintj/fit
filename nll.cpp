#include <cmath>
#include "nll.h"

nll::nll(pdf_base & p, dataset & d):
	m_pdf(p),
	m_data(d)
{
}

nll::~nll()
{
}

double nll::operator()(const std::vector<double> & par) const
{
	m_pdf.read_from_pars(par);
	double sum = m_pdf.log_sum_on(m_data) + log(m_pdf.norm())*m_data.size();
	//std::cout << "par0 = " << par.at(0) << ", par1 = " << par.at(1) << ", norm = " << m_pdf.norm() << ", nll = " << -sum << std::endl;
	return -sum;
}
