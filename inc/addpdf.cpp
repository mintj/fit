#include <iostream>
#include "TMath.h"
#include "chi2fcn.h"
#include "datahist.h"
#include "dataset.h"
#include "addpdf.h"
#include "variable.h"

addpdf::addpdf(const std::vector<pdf *> plist, const std::vector<variable *> flist, dataset * normset):
	pdf(),
	m_plist(plist),
	m_flist(flist)
{
	assert(plist.size()>1 && plist.size() == flist.size()+1);
	m_normset = normset;
	init();
}

addpdf::~addpdf()
{
}

void addpdf::chi2fit(datahist * data, bool minos_err)
{
	chi2fcn * chi2 = create_chi2(data);
	chi2->minimize(minos_err);
}

chi2fcn * addpdf::create_chi2(datahist * data)
{
	m_chi2.reset(new chi2fcn(this, data));
	return m_chi2.get();
}

double addpdf::evaluate(const double * x)
{
	double v = 0;
	double ftot = 0;
	for (size_t u = 0; u < m_plist.size(); ++u) {
		double f;
		if (u < m_flist.size()) {
			f = m_flist.at(u)->value();
			ftot += f;
		}
		else {
			f = 1-ftot;
		}
		double raw = m_plist.at(u)->evaluate(x);
		double scale = m_plist.at(u)->norm();
		v += f * scale * raw;
	}
	return v;
}

void addpdf::init()
{
	for (pdf * p: m_plist) {
		m_dim = p->dim();
		p->set_normset(m_normset);
		m_varlist.insert(m_varlist.end(), p->get_vars().begin(), p->get_vars().end());
		m_lastvalue.insert(m_lastvalue.end(), p->get_lastvalues().begin(), p->get_lastvalues().end());
	}
	for (variable * v: m_flist) {
		m_varlist.push_back(v);
		m_lastvalue.push_back(v->value()-0.1);
	}
}

//double addpdf::integral(double a, double b, int n)
//{
//	//double intval = 0;
//	//double ftot = 0;
//	//for (size_t u = 0; u < m_plist.size(); ++u) {
//	//	double f;
//	//	if (u < m_flist.size()) {
//	//		f = m_flist.at(u)-value();
//	//		ftot += f;
//	//	}
//	//	else {
//	//		f = 1-ftot;
//	//	}
//	//	intval += f * m_plist.at(u)->integral(a, b, n);
//	//}
//	//return intval;
//}

//double addpdf::log_sum(dataset * data)
//{
//	return pdf::log_sum(data);// + log(TMath::Poisson(data->nevt(), nevt()));
//}

//double addpdf::nevt()
//{
//	double ntot = 0;
//	for (size_t u = 0; u < m_flist.size(); ++u) {
//		ntot += m_flist.at(u)->value();
//	}
//	return ntot;
//}

void addpdf::set_normset(dataset * normset)
{
	m_normset = normset;
	for (auto * p: m_plist) {
		p->set_normset(normset);
	}
}
