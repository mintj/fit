#include <iostream>
#include <cmath>
#include "abspdf.h"
#include "datahist.h"
#include "dataset.h"
#include "nllfcn.h"
#include "util.h"
#include "variable.h"

nllfcn::nllfcn(abspdf * p, absdata * d):
	absfcn(p, d)
{
}

nllfcn::~nllfcn()
{
}

bool nllfcn::init()
{
	m_arr_rdata.resize(0);
	m_arr_logsum.resize(0);
	
	std::vector<std::vector<int>> idvv;
	for (size_t u = 0; u < m_datalist.size(); ++u) {
		abspdf * p = m_pdflist[u];
		absdata * d = m_datalist[u];
		
		std::vector<int> idv;
		for (variable * var: p->var_list()) {
			int dim = d->get_dim(var);
			if (dim < 0) {
				MSG_ERROR("data '", d->name(), "' does not contain all variables of pdf '", p->name(), "'");
				return false;
			}
			idv.push_back(dim);
		}
		idvv.push_back(idv);
	}
	
	for (size_t u = 0; u < m_datalist.size(); ++u) {
		abspdf * p = m_pdflist[u];
		absdata * d = m_datalist[u];
		
		std::vector<double> rdata;
		for (size_t v = 0; v < d->size(); ++v) {
			for (size_t id: idvv[u]) {
				rdata.push_back(d->at(v)[id]);
			}
		}
		m_arr_rdata.push_back(rdata);
	}

	return true;
}

void nllfcn::minimize(bool minos_err)
{
	if (init()) {
		absfcn::minimize(minos_err);
	}
}

double nllfcn::operator()(const std::vector<double> & par) const
{
	for (size_t u = 0; u < m_paralist.size(); ++u) {
		m_paralist[u]->set_value(par[u]);
		//std::cout << u << " " << par[u] << std::endl;
	}

	bool newcall = false;
	if (!m_arr_logsum.size()) {
		m_arr_logsum.resize(m_arr_rdata.size());
		newcall = true;
	}

	double nll = 0;
	for (size_t u = 0; u < m_pdflist.size(); ++u) {
		abspdf * p = m_pdflist[u];
		absdata * d = m_datalist[u];
		if (p->updated() || newcall) {
			double logsum = 0;
			newcall = false;

			dataset * ds = dynamic_cast<dataset *>(d);
			if (ds) {
				for (size_t v = 0; v < d->size(); ++v) {
					double rv = p->evaluate(&m_arr_rdata[u][v]);
					if (rv > 0) {
						logsum += log(rv) * d->weight(v);
					}
					else {
						MSG_WARNING("non-positive pdf value is ignored during nll fit, it might cause failure");
					}
				}
				logsum += log(p->norm()) * d->nevt();
			}

			datahist * dh = dynamic_cast<datahist *>(d);
			if (dh) {
				double ntot = dh->nevt();
				double min = dh->min();
				double max = dh->max();
				double sf = ntot / p->recursive_simpson(min, max);
				for (int u = 0; u < dh->size(); ++u) {
					double nobs = dh->weight(u);
					double nfit = sf * p->recursive_simpson(dh->edge_lo(u), dh->edge_hi(u));
					//if (nobs) logsum -= (nfit-nobs)*(nfit-nobs)/nobs;
					//else if (nfit) logsum -= (nfit-nobs)*(nfit-nobs)/nfit;
					double prob = 0;
					if (nobs) prob = TMath::Poisson(nfit, nobs);
					else if (nfit) prob = TMath::Poisson(nobs, nfit);
					if (prob > 0) logsum += log(prob);
				}
			}
			
			m_arr_logsum[u] = logsum;
		}
		nll -= m_arr_logsum[u];
	}
	return nll;
}
