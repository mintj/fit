#include <iostream>
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnUserParameters.h"
#include "dataset.h"
#include "fcn.h"
#include "nllfcn.h"
#include "pdf.h" 
#include "variable.h"

pdf::pdf():
	m_norm(1),
	m_status(-1),
	m_updated(true)
{
}

pdf::pdf(size_t dim, const std::vector<variable *> & var, dataset * normset):
	m_dim(dim),
	m_norm(1),
	m_status(-1),
	m_updated(true),
	m_normset(normset)
{
	for (variable * v: var) {
		m_varlist.push_back(v);
		m_lastvalue.push_back(v->value());
	}
}

pdf::~pdf()
{
}

nllfcn * pdf::create_nll(dataset * data)
{
	m_nll.reset(new nllfcn(this, data));
	return m_nll.get();
}

fitresult * pdf::fit(dataset * data, bool minos_err)
{
	nllfcn * nll = create_nll(data);
	ROOT::Minuit2::MnUserParameters upar;
	for (variable * v: nll->get_var_list()) {
		//std::cout << "aaa: " << v->name() << " " << v->value() << " (" << v->limit_down() << ", " << v->limit_up() << ") " << v->err() << std::endl;
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*nll, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	std::cout << min << std::endl;
	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*nll, min);
		std::cout << "1-sigma minos errors: " << std::endl;
		for (size_t u = 0; u < nll->get_var_list().size(); ++u) {
			std::pair<double, double> e = minos(u);
			const char * name = nll->get_var(u)->name();
			std::cout << name << " " << min.UserState().Value(name) << " " << e.first << " " << e.second << std::endl;
		}
	}

	return 0;
}
double pdf::get_lastvalue(int n)
{
	return m_lastvalue[n];
}

std::vector<double> & pdf::get_lastvalues()
{
	return m_lastvalue;
}

double pdf::get_par(int n)
{
	return m_varlist[n]->value();
}

variable * pdf::get_var(int n)
{
	return m_varlist[n];
}

std::vector<variable *> & pdf::get_vars()
{
	return m_varlist;
}

// 1d dimensional integral
double pdf::integral(double a, double b, int n)
{
	if (!m_normset || !m_normset->size()) return 0;

	int sign = (a < b) ? 1 : -1;
	double min = (a < b) ? a : b;
	double max = (a < b) ? b : a;
	double intval = 0;
	for (size_t u = 0; u < m_normset->size(); ++u) {
		double d = m_normset->at(u)[n];
		if (d > min && d < max) {
			intval += evaluate(m_normset->at(u));
		}
	}
	return sign*intval*norm()/m_normset->size();
}

double pdf::log_sum(dataset * data)
{
	if (!data) return 1e-20;

	double log_sum = 0;
	for (size_t u = 0; u < data->size(); ++u) {
		double v = evaluate(data->at(u));
		if (v > 0) log_sum += log(v);
	}
	return log_sum;
}

double pdf::norm()
{
	m_status = normalize();

	if (m_status) {
		std::cout << "[pdf] error: pdf not normalized (status = " << m_status << ")" << std::endl;
	}

	return m_norm;
}

int pdf::normalize()
{
	if (updated()) {
		m_updated = false;
		m_norm = 1;
		if (!m_normset || !m_normset->size()) return -1;

		double s = sum(m_normset);
		if (s == 0) return 1;
		else {
			m_norm = m_normset->size()/s;
			return 0;
		}
	}
	return m_status;
}

double pdf::operator()(const double * x)
{
	return norm()*evaluate(x);
}

void pdf::set_normset(dataset * normset)
{
	if (m_normset != normset) {
		m_normset = normset;
		m_updated = true;
	}
}

double pdf::sum(dataset * data)
{
	if (!data) return 0;

	double s = 0;
	for (size_t u = 0; u < data->size(); ++u) {
		double v = evaluate(data->at(u));
		if (v >= 0) s += v;
	}
	return s;
}

bool pdf::updated()
{
	for (size_t u = 0; u < m_varlist.size(); ++u) {
		if (m_varlist.at(u)->value() != m_lastvalue.at(u)) {
			m_lastvalue[u] = m_varlist.at(u)->value();
			m_updated |= true;
		}
	}
	return m_updated;
}
