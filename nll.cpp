#include <iostream>
#include <cmath>
#include "dataset.h"
#include "extpdf.h"
#include "nll.h"
#include "pdf.h"
#include "variable.h"

nll::nll(pdf * p, dataset * d):
	m_pdflist({p}),
	m_datalist({d})
{
	init();
}

nll::nll(const std::vector<pdf *> plist, const std::vector<dataset *> dlist):
	m_pdflist(plist.cbegin(), plist.cend()),
	m_datalist(dlist.cbegin(), dlist.cend())
{
	init();
}

nll::~nll()
{
}

void nll::init()
{
	for (pdf * p: m_pdflist) {
		for (variable * v: p->get_vars()) {
			if (m_vcount.find(v) == m_vcount.end()) {
				m_varlist.push_back(v);
			}
			++m_vcount[v];
		}
	}
}

double nll::operator()(const std::vector<double> & par) const
{
	double sum_nll = 0;

	for (size_t u = 0; u < m_varlist.size(); ++u) {
		m_varlist[u]->set_value(par[u]);
		//cout << u << " " << par[u] << endl;
	}

	for (size_t u = 0; u < m_pdflist.size(); ++u) {
		pdf * p = m_pdflist[u];
		dataset * d = m_datalist[u];
		sum_nll -= p->log_sum(d);
		sum_nll -= log(p->norm())*d->size();
	}

	return sum_nll;
}
