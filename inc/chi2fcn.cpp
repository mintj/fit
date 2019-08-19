#include <iostream>
#include <cmath>
#include "abspdf.h"
#include "chi2fcn.h"
#include "datahist.h"
#include "util.h"
#include "variable.h"

chi2fcn::chi2fcn(abspdf * p, datahist * d):
	absfcn(p, d)
{
}

chi2fcn::~chi2fcn()
{
}

bool chi2fcn::init()
{
	m_arr_chi2.resize(m_pdflist.size(), -1);

	for (size_t u = 0; u < m_pdflist.size(); ++u) {
		abspdf * p = m_pdflist[u];
		absdata * d = m_datalist[u];
		if (p->dim() > 1) {
			MSG_ERROR("chi2fit is not supported for high dimensional pdf");
			return false;
		}
		else if (d->get_dim(p->var_list()[0]) < 0) {
			MSG_ERROR("data '", d->name(), "' does not contain variable '", p->var_list()[0]->name(), "'");
			return false;
		}
	}

	return true;
}

void chi2fcn::minimize(bool minos_err)
{
	if (init()) {
		absfcn::minimize(minos_err);
	}
}

double chi2fcn::operator()(const std::vector<double> & par) const
{
	double chi2 = 0;

	for (size_t u = 0; u < m_paralist.size(); ++u) {
		m_paralist[u]->set_value(par[u]);
		//std::cout << u << " " << par[u] << std::endl;
	}

	for (size_t u = 0; u < m_pdflist.size(); ++u) {
		abspdf * p = m_pdflist.at(u);
		datahist * d = dynamic_cast<datahist *>(m_datalist.at(u));
		
		if (p->updated() || m_arr_chi2[u] < 0) {
			m_arr_chi2[u] = 0;
			double min = d->min();
			double max = d->max();
			double sf = d->nevt() / p->recursive_simpson(min, max);
			for (size_t v = 0; v < d->size(); ++v) {
				double nobs = d->weight(v);
				double nfit = sf * p->recursive_simpson(d->edge_lo(v), d->edge_hi(v));
				double err = (nobs < nfit) ? d->err_up(v) : d->err_down(v);
				m_arr_chi2[u] += (nfit-nobs)*(nfit-nobs)/err/err;
			}
		}
		chi2 += m_arr_chi2[u];
	}

	return chi2;
}
