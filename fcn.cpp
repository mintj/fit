#include <iostream>
#include <cmath>
#include "Minuit2/MnUserParameters.h"
#include "datahist.h"
#include "dataset.h"
#include "extpdf.h"
#include "fcn.h"
#include "pdf.h"
#include "variable.h"

fcn::fcn(pdf * p, dataset * d):
	m_pdflist({p}),
	m_datalist({d})
{
	init();
}

fcn::fcn(const std::vector<pdf *> plist, const std::vector<dataset *> dlist):
	m_pdflist(plist.cbegin(), plist.cend()),
	m_datalist(dlist.cbegin(), dlist.cend())
{
	init();
}

fcn::fcn(const std::vector<extpdf *> plist, const std::vector<datahist *> dlist):
	m_pdflist(plist.cbegin(), plist.cend()),
	m_datalist(dlist.cbegin(), dlist.cend())
{
	init();
}

fcn::~fcn()
{
}

void fcn::init()
{
	for (pdf * p: m_pdflist) {
		for (variable * v: p->get_vars()) {
			if (!v->constant()) {
				if (m_vcount.find(v) == m_vcount.end()) {
					m_varlist.push_back(v);
				}
				++m_vcount[v];
			}
		}
	}
}

void fcn::minimize(bool minos_err)
{
	ROOT::Minuit2::MnUserParameters upar;
	for (variable * v: get_var_list()) {
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*this, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	for (variable * v: get_var_list()) {
		v->set_value(upar.Value(v->name()));
		v->set_err(upar.Error(v->name()));
	}
	std::cout << min << std::endl;

	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*this, min);
		std::cout << "1-sigma minos errors: " << std::endl;
		for (size_t u = 0; u < get_var_list().size(); ++u) {
			std::pair<double, double> e = minos(u);
			variable * v = get_var(u);
			const char * name = v->name();
			std::cout << name << " " << get_var(u)->value() << " " << e.first << " " << e.second << std::endl;
			v->set_err_down(e.first);
			v->set_err_up(e.second);
		}
	}
}
