#include <iostream>
#include <cmath>
#include "addpdf.h"
#include "chi2fcn.h"
#include "datahist.h"
#include "fcn.h"
#include "pdf.h"
#include "variable.h"

chi2fcn::chi2fcn(addpdf * p, datahist * d):
	fcn(p, d)
{
}

chi2fcn::chi2fcn(const std::vector<addpdf *> plist, const std::vector<datahist *> dlist):
	fcn(plist, dlist)
{
}

chi2fcn::~chi2fcn()
{
}

double chi2fcn::operator()(const std::vector<double> & par) const
{
	double chi2 = 0;

	for (size_t u = 0; u < m_varlist.size(); ++u) {
		m_varlist[u]->set_value(par[u]);
		//std::cout << u << " " << par[u] << std::endl;
	}

	for (size_t u = 0; u < m_pdflist.size(); ++u) {
		pdf * p = m_pdflist.at(u);
		datahist * d = dynamic_cast<datahist *>(m_datalist.at(u));
		double nevt = d->nevt();
		for (size_t v = 0; v < d->size(); ++v) {
			double nobs = d->weight(v);
			double err_u = d->err_up(v);
			double err_d = d->err_down(v);
			double nfit = nevt * p->integral(d->edge_lo(v), d->edge_hi(v), 0);
			if (nfit > nobs && err_u) {
				chi2 += pow(nfit-nobs, 2)/err_u/err_u;
			}
			else if (nfit < nobs && err_d) {
				chi2 += pow(nfit-nobs, 2)/err_d/err_d;
			}
			else if (nfit) {
				chi2 += pow(nfit-nobs, 2)/nfit;
			}
		}
	}

	return chi2;
}
