#include <iostream>
#include "TMath.h"
#include "chi2fcn.h"
#include "datahist.h"
#include "dataset.h"
#include "extpdf.h"
#include "variable.h"

extpdf::extpdf(pdf * p, variable * n, dataset * normset):
	pdf(),
	m_plist(1, p),
	m_nlist(1, n)
{
	m_normset = normset;
	init();
}

extpdf::extpdf(const std::vector<pdf *> plist, const std::vector<variable *> nlist, dataset * normset):
	pdf(),
	m_plist(plist),
	m_nlist(nlist)
{
	m_normset = normset;
	init();
}

extpdf::~extpdf()
{
}

void extpdf::chi2fit(datahist * data, bool minos_err)
{
	chi2fcn * chi2 = create_chi2(data);
	chi2->minimize(minos_err);
}

chi2fcn * extpdf::create_chi2(datahist * data)
{
	m_chi2.reset(new chi2fcn(this, data));
	return m_chi2.get();
}

double extpdf::evaluate(const double * x)
{
	double ntot = nevt();
	if (ntot) {
		double v = 0;
		for (size_t u = 0; u < m_plist.size(); ++u) {
			double vraw = m_plist.at(u)->evaluate(x);
			double scale = m_plist.at(u)->norm();
			double n = m_nlist.at(u)->value();
			//std::cout << "(raw, norm, nevt) = (" << vraw << ", " << scale << ", " << n << ")" << std::endl;
			v += n * scale * vraw;
		}
		return v/ntot;
	}
	else return 0;
}

void extpdf::init()
{
	for (pdf * p: m_plist) {
		m_dim = p->dim();
		p->set_normset(m_normset);
		m_varlist.insert(m_varlist.end(), p->get_vars().begin(), p->get_vars().end());
		m_lastvalue.insert(m_lastvalue.end(), p->get_lastvalues().begin(), p->get_lastvalues().end());
	}
	for (variable * v: m_nlist) {
		m_varlist.push_back(v);
		m_lastvalue.push_back(v->value());
	}
}

double extpdf::integral(double a, double b, int n)
{
	double ntot = nevt();
	if (ntot) {
		double intval = 0;
		for (size_t u = 0; u < m_nlist.size(); ++u) {
			intval += m_nlist.at(u)->value() * m_plist.at(u)->integral(a, b, n);
		}
		intval /= ntot;
		return intval;
	}
	else return 0;
}

double extpdf::log_sum(dataset * data)
{
	return pdf::log_sum(data) + log(TMath::Poisson(data->nevt(), nevt()));
}

double extpdf::nevt()
{
	double ntot = 0;
	for (size_t u = 0; u < m_nlist.size(); ++u) {
		ntot += m_nlist.at(u)->value();
	}
	return ntot;
}

void extpdf::set_normset(dataset * normset)
{
	m_normset = normset;
	for (auto * p: m_plist) {
		p->set_normset(normset);
	}
}
