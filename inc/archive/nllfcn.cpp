#include <iostream>
#include <cmath>
#include "addpdf.h"
#include "dataset.h"
#include "fcn.h"
#include "nllfcn.h"
#include "pdf.h"
#include "variable.h"

nllfcn::nllfcn(pdf * p, dataset * d):
	fcn(p, d),
	m_arr_logsum(1),
	m_arr_norm(1, -1)
{
}

nllfcn::~nllfcn()
{
}

void nllfcn::add(pdf * p, dataset * d)
{
	fcn::add(p, d);
	m_arr_logsum.push_back(1);
	m_arr_norm.push_back(-1);
}

double nllfcn::operator()(const std::vector<double> & par) const
{
	for (size_t u = 0; u < m_varlist.size(); ++u) {
		m_varlist[u]->set_value(par[u]);
		//cout << u << " " << par[u] << endl;
	}

	double nll = 0;
	for (size_t u = 0; u < m_pdflist.size(); ++u) {
		pdf * p = m_pdflist[u];
		dataset * d = m_datalist[u];
		if (p->updated() || m_arr_norm[u] < 0) {
			m_arr_logsum[u] = p->log_sum(d);
			m_arr_norm[u] = p->norm();
		}
		nll -= m_arr_logsum[u];
		nll -= log(m_arr_norm[u])*d->nevt();
	}
	return nll;
}
