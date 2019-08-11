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

fitresult * extpdf::chi2fit(datahist * data, bool minos_err)
{
	chi2fcn * chi2 = create_chi2(data);
	ROOT::Minuit2::MnUserParameters upar;
	for (variable * v: chi2->get_var_list()) {
		//std::cout << "aaa: " << v->name() << " " << v->value() << " (" << v->limit_down() << ", " << v->limit_up() << ") " << v->err() << std::endl;
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*chi2, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	std::cout << min << std::endl;
	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*chi2, min);
		std::cout << "1-sigma minos errors: " << std::endl;
		for (size_t u = 0; u < chi2->get_var_list().size(); ++u) {
			std::pair<double, double> e = minos(u);
			const char * name = chi2->get_var(u)->name();
			std::cout << name << " " << min.UserState().Value(name) << " " << e.first << " " << e.second << std::endl;
		}
	}

	return 0;
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
			double raw_value = m_plist.at(u)->evaluate(x);
			double norm_factor = m_plist.at(u)->norm();
			double n = m_nlist.at(u)->value();
			//std::cout << "(raw, norm, nevt) = (" << raw_value << ", " << norm_factor << ", " << n << ")" << std::endl;
			v += n * norm_factor * raw_value;
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
	return pdf::log_sum(data) + log(TMath::Poisson(nevt(), data->nevt()));
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

//double extpdf::norm()
//{
//	
//}
//
//double extpdf::sum(dataset * data)
//{
//	return 0;
//}
