#include "datahist.h"
#include "dataset.h"
#include "pdf.h" 
#include "variable.h"

pdf::pdf(size_t dim, const std::vector<variable *> & var, dataset * normset):
	m_dim(dim),
	m_normset(normset)
{
	for (size_t u = 0; u < var.size(); ++u) {
		if (m_vmap.find(var[u]->name) == m_vmap.end()) {
			m_vmap[var[u]->name] = m_var.size();
			m_var.push_back(var[u]);
		}
	}
	m_npar = m_var.size();
}

pdf::~pdf()
{
}

variable * pdf::get_var(const char * name)
{
	if (m_vmap.find(name) == m_vmap.end()) return 0;
	else return m_var[m_vmap[name]];
}

double pdf::log_sum(dataset * data, const std::vector<double> & par)
{
	if (!data) return 1e-20;

	double log_sum = 0;
	for (size_t u = 0; u < data->size(); ++u) {
		double v = evaluate(data->at(u), par);
		if (v > 0) log_sum += log(v);
	}
	return log_sum;
}

double pdf::norm(const std::vector<double> & par)
{
	if (!m_normset || !m_normset->size()) return 1;
	
	double s = sum(m_normset, par);
	if (s == 0) return 1;
	else return m_normset->size()/s;
}

double pdf::sum(dataset * data, const std::vector<double> & par)
{
	if (!data) return 0;

	double s = 0;
	for (size_t u = 0; u < data->size(); ++u) {
		double v = evaluate(data->at(u), par);
		if (v >= 0) s += v;
	}
	return s;
}
