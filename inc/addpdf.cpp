#include <iostream>
#include "TMath.h"
#include "chi2fcn.h"
#include "datahist.h"
#include "dataset.h"
#include "addpdf.h"
#include "variable.h"

addpdf::addpdf(const std::vector<pdf *> plist, const std::vector<variable *> flist):
	pdf(),
	m_plist(plist),
	m_flist(flist),
	m_frac(plist.size(), 0)
{
	assert(plist.size()>1 && plist.size() == flist.size()+1);
	m_normset = plist[0]->normset();
	m_dim = plist[0]->dim();
	init();
}

addpdf::~addpdf()
{
}

void addpdf::calculate_frac()
{
	double ftot = 0;
	for (size_t u = 0; u < m_flist.size(); ++u) {
		m_frac[u] = m_flist.at(u)->value();
		ftot += m_frac[u];
	}
	m_frac[m_flist.size()] = 1-ftot;
}

double addpdf::evaluate(const double * x)
{
	calculate_frac();
	double v = 0;
	for (size_t u = 0; u < m_plist.size(); ++u) {
		double f = m_frac[u];
		double raw = m_plist.at(u)->evaluate(x);
		double scale = m_plist.at(u)->norm();
		v += f * scale * raw;
	}
	return v;
}

void addpdf::init()
{
	for (pdf * p: m_plist) {
		if (p->dim() != m_dim) {
			std::cout << "[addpdf] error: all pdf must have the same dimension" << std::endl;
		}
		if (p->normset() != m_normset) {
			std::cout << "[addpdf] error: all pdf must have the same normset" << std::endl;
		}
		m_varlist.insert(m_varlist.end(), p->get_vars().begin(), p->get_vars().end());
		m_lastvalue.insert(m_lastvalue.end(), p->get_lastvalues().begin(), p->get_lastvalues().end());
	}
	for (variable * v: m_flist) {
		m_varlist.push_back(v);
		m_lastvalue.push_back(v->value()-0.1);
	}
}

double addpdf::integral(double a, double b, int n)
{
	calculate_frac();
	double tot = 0;
	for (size_t u = 0; u < m_plist.size(); ++u) {
		double f = m_frac[u];
		tot += f * m_plist[u]->integral(a, b, n);
	}
	return tot;
}

void addpdf::set_normset(dataset & normset)
{
	m_normset = &normset;
	for (auto * p: m_plist) {
		p->set_normset(normset);
	}
}
