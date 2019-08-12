#include <iostream>
#include <cmath>
#include "dataset.h"
#include "extpdf.h"
#include "fcn.h"
#include "nllfcn.h"
#include "pdf.h"
#include "variable.h"

nllfcn::nllfcn(pdf * p, dataset * d):
	fcn(p, d)
{
}

nllfcn::nllfcn(const std::vector<pdf *> plist, const std::vector<dataset *> dlist):
	fcn(plist, dlist)
{
}

nllfcn::~nllfcn()
{
}

double nllfcn::operator()(const std::vector<double> & par) const
{
	double nll = 0;

	for (size_t u = 0; u < m_varlist.size(); ++u) {
		m_varlist[u]->set_value(par[u]);
		//cout << u << " " << par[u] << endl;
	}

	for (size_t u = 0; u < m_pdflist.size(); ++u) {
		pdf * p = m_pdflist[u];
		dataset * d = m_datalist[u];
		nll -= p->log_sum(d);
		cout << "pnorm and dnevt: " << p->norm() << " " << d->nevt() << endl;
		nll -= log(p->norm())*d->nevt();
	}

	return nll;
}
