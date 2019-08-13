#include <iostream>
#include <cmath>
#include "chi2fcn.h"
#include "datahist.h"
#include "fcn.h"
#include "pdf.h"
#include "variable.h"

chi2fcn::chi2fcn(pdf * p, datahist * d):
	fcn(p, d)
{
	update_data(p, d);
}

chi2fcn::~chi2fcn()
{
}

void chi2fcn::add(pdf * p, datahist * d)
{
	fcn::add(p, d);
	update_data(p, d);
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
		
		double nfit_tot = 0;
		std::vector<double> nfit_vec(d->size(), 0);
		for (size_t v = 0; v < d->size(); ++v) {
			for (size_t w = 0; w < m_data[u][v].size(); ++w) {
				nfit_vec[v] += p->evaluate(m_data[u][v][w]);
			}
			nfit_tot += nfit_vec[v];
		}
		
		double nevt = d->nevt();
		for (size_t v = 0; v < d->size(); ++v) {
			double nobs = d->weight(v);
			double err_u = d->err_up(v);
			double err_d = d->err_down(v);
			double nfit = nevt * nfit_vec[v] / nfit_tot;
			
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

void chi2fcn::update_data(pdf * p, datahist * d)
{
	dataset * ns = p->normset();
	std::vector<std::vector<double *>> vvp(d->size());
	for (size_t v = 0; v < ns->size(); ++v) {
		double x = ns->at(v)[0];
		int bin = d->find_bin(x);
		if (bin > 0 && bin <= d->size()) {
			vvp[bin-1].push_back(ns->at(v));
		}
	}
	m_data.push_back(std::move(vvp));
}
