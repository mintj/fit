#include <iostream>
#include <cmath>
#include <cstring>
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnUserParameters.h"
#include "absdata.h"
#include "absfcn.h"
#include "abspdf.h"
#include "util.h"
#include "variable.h"

absfcn::absfcn(abspdf * p, absdata * d)
{
	add(p, d);
}

absfcn::~absfcn()
{
}

void absfcn::add(abspdf * p, absdata * d)
{
	for (variable * v: p->var_list()) {
		if (d->get_dim(v) < 0) {
			MSG_ERROR("data '", d->name(), "' does not contain all variables of pdf '", p->name(), "'");
			return;
		}
	}

	m_pdflist.push_back(p);
	m_datalist.push_back(d);
	for (variable * v: p->para_list()) {
		if (!v->constant()) {
			if (m_count.find(v) == m_count.end()) {
				m_paralist.push_back(v);
			}
			++m_count[v];
		}
	}
}

void absfcn::minimize(bool minos_err)
{
	ROOT::Minuit2::MnUserParameters upar;
	for (variable * v: m_paralist) {
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*this, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	for (variable * v: m_paralist) {
		// seems that fit value is automatically set by minuit
		v->set_value(min.UserState().Value(v->name()));
		v->set_err(min.UserState().Error(v->name()));
	}
	std::cout << min << std::endl;

	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*this, min);
		std::cout << "1-sigma minos errors: " << std::endl;
		for (size_t u = 0; u < m_paralist.size(); ++u) {
			std::pair<double, double> e = minos(u);
			variable * v = m_paralist[u];
			const char * name = v->name();
			std::cout << name << " " << min.UserState().Value(v->name()) << " " << e.first << " " << e.second << std::endl;
			v->set_value(min.UserState().Value(v->name()));
			v->set_err_down(e.first);
			v->set_err_up(e.second);
		}
	}
}
