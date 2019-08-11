#include <iostream>

#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnUserParameters.h"

#include "datahist.h"
#include "dataset.h"
#include "nll.h"
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
	for (size_t u = 0; u < var.size(); ++u) {
		variable * v = var.at(u);
		m_var_ext.push_back(v);
		if (m_vcount.find(v) == m_vcount.end()) {
			m_var_int.push_back(v);
			m_var_int_lastvalue.push_back(0);
			++m_vcount[v];
		}
	}
}

pdf::~pdf()
{
}

nll * pdf::create_nll(dataset * data)
{
	m_nll.reset(new nll(this, data));
	return m_nll.get();
}

//fitresult * pdf::fit(dataset * data, bool minos)
void pdf::fit(dataset * data, bool minos_err)
{
	nll * fcn = create_nll(data);
	ROOT::Minuit2::MnUserParameters upar;
	for (size_t u = 0; u < npar_int(); ++u) {
		auto v = get_var_int(u);
		cout << "aaa: " << v->name() << " " << v->value() << " (" << v->limit_down() << ", " << v->limit_up() << ") " << v->err() << endl;
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*fcn, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	cout << min << endl;
	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*fcn, min);
		cout << "1-sigma minos errors: " << endl;
		for (size_t u = 0; u < npar_int(); ++u) {
			pair<double, double> e = minos(u);
			const char * name = get_var_int(u)->name();
			cout << name << " " << min.UserState().Value(name) << " " << e.first << " " << e.second << endl;
		}
	}
}

double pdf::get_par_ext(int n)
{
	return get_var_ext(n)->value();
}

double pdf::get_par_int(int n)
{
	return get_var_int(n)->value();
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
	for (size_t u = 0; u < m_var_int.size(); ++u) {
		if (m_var_int.at(u)->value() != m_var_int_lastvalue.at(u)) {
			m_var_int_lastvalue[u] = m_var_int.at(u)->value();
			m_updated |= true;
		}
	}
	return m_updated;
}
