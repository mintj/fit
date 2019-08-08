#include <iostream>
#include "datahist.h"
#include "dataset.h"
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
