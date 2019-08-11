#include <iostream>
#include <cmath>
#include "dataset.h"
#include "extpdf.h"
#include "nll.h"
#include "pdf.h"
#include "variable.h"

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
	double v = 0;

	for (size_t u = 0; u < m_pdf->npar_int(); ++u) {
		m_pdf->get_var_int(u)->set_value(par[u]);
		//cout << u << " " << par[u] << endl;
	}

	v -= m_pdf->log_sum(m_data);
	v -= log(m_pdf->norm())*m_data->size();

	return v;
}
