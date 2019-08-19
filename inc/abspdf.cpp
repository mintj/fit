#include <iostream>
#include "TString.h"
//#include "Minuit2/FunctionMinimum.h"
//#include "Minuit2/MnMigrad.h"
//#include "Minuit2/MnMinos.h"
//#include "Minuit2/MnUserParameters.h"
#include "TRandom3.h"
#include "abspdf.h" 
#include "datahist.h"
#include "dataset.h"
#include "nllfcn.h"
#include "plot.h"
#include "plotcmd.h"
#include "util.h"
#include "variable.h"

abspdf::abspdf(const char * name, const std::vector<const char *> & varlist, const std::vector<const char *> & paralist)
{
	assert(varlist.size() > 0);
	m_name = name;
	m_dim = varlist.size();
	m_npara = paralist.size();
	for (auto name: varlist) {
		variable * v = variable::var(name);
		m_varlist.push_back(v);
		m_max.push_back(v->limit_up());
		m_min.push_back(v->limit_down());
	}
	for (auto name: paralist) {
		variable * v = variable::var(name);
		m_paralist.push_back(v);
		m_lastvalue.push_back(v->value()-0.1);
	}
}

abspdf::~abspdf()
{
}

//void abspdf::chi2fit(datahist & data, bool minos_err)
//{
//	chi2fcn * chi2 = create_chi2(&data);
//	chi2->minimize(minos_err);
//}
//
nllfcn * abspdf::create_nll(absdata * data)
{
	m_nll.reset(new nllfcn(this, data));
	return m_nll.get();
}

//
//chi2fcn * abspdf::create_chi2(datahist * data)
//{
//	m_chi2.reset(new chi2fcn(this, data));
//	return m_chi2.get();
//}
//
void abspdf::fit(absdata & data, bool minos_err)
{
	nllfcn * nll = create_nll(&data);
	nll->minimize(minos_err);
}

//double abspdf::get_lastvalue(int n)
//{
//	return m_lastvalue[n];
//}
//
//std::vector<double> & abspdf::get_lastvalues()
//{
//	return m_lastvalue;
//}
//

dataset * abspdf::generate(variable & x, size_t nevt, size_t ns)
{
	return generate_eliminate_ambiguity({x}, nevt, ns);
}

dataset * abspdf::generate(const std::vector<variable> & xvec, size_t nevt, size_t ns)
{
	return generate_eliminate_ambiguity(xvec, nevt, ns);
}

dataset * abspdf::generate_eliminate_ambiguity(const std::vector<variable> & xvec, size_t nevt, size_t ns)
{
	if (!xvec.size()) {
		MSG_ERROR("no variable specified");
		return 0;
	}

	for (size_t u = 0; u < xvec.size(); ++u) {
		if (get_dim(xvec[u].name()) < 0) {
			MSG_ERROR("pdf '", m_name, "' does not contain variable '", xvec[u].name(), "'");
			return 0;
		}
	}
	
	TRandom3 rndm(nevt);
	
	double prob_max = -1;
	std::vector<double> dp(m_dim, 0);
	for (size_t u = 0; u < ns; ++u) {
		for (size_t v = 0; v < m_dim; ++v) {
			dp[v] = rndm.Uniform(m_min[v], m_max[v]);
		}
		double prob = evaluate(&dp[0]);
		if (prob_max < prob) prob_max = prob;
	}
	prob_max *= 1.1;

	const char * name = Form("data_of_[%s]", xvec[0].name());
	for (int u = 1; u < xvec.size(); ++u) {
		name = Form("%s_[%s]", name, xvec[u].name());
	}
	dataset * data = new dataset(name, xvec);
	
	while (data->size() < nevt) {
		for (size_t v = 0; v < m_dim; ++v) {
			dp[v] = rndm.Uniform(m_min[v], m_max[v]);
		}
		double prob = evaluate(&dp[0]);
		if (rndm.Rndm() < prob/prob_max) {
			data->add(&dp[0], 1);
		}
	}

	return data;
}

double abspdf::get_para(int n)
{
	return m_paralist[n]->value();
}
//
//variable * abspdf::get_var(int n)
//{
//	return m_varlist[n];
//}
//
//std::vector<variable *> & abspdf::get_vars()
//{
//	return m_varlist;
//}
//
//// 1d dimensional integral
//double abspdf::integral(double a, double b, int n)
//{
//	if (!m_normset || !m_normset->nevt()) return 0;
//
//	int sign = (a < b) ? 1 : -1;
//	double min = (a < b) ? a : b;
//	double max = (a < b) ? b : a;
//	double intval = 0;
//	for (size_t u = 0; u < m_normset->nevt(); ++u) {
//		double d = m_normset->at(u)[n];
//		if (d > min && d < max) {
//			intval += evaluate(m_normset->at(u)) * m_normset->weight(u); //TODO: check here
//		}
//	}
//	return sign*intval*norm()/m_normset->nevt();
//}

int abspdf::get_dim(const char * vname)
{
	for (int u = 0; u < m_varlist.size(); ++u) {
		if (!strcmp(vname, m_varlist[u]->name())) return u;
	}
	return -1;
}

int abspdf::get_dim(variable * v)
{
	return get_dim(v->name());
}

double abspdf::integral1d(double a, double b, double eps)
{
	// important note: this method should only be used for 1d pdf
	// it can be safely used in chi2/nll fit to datahist: simply notice that datahist is 1d, hence the pdf must be 1d
	if (m_dim > 1) {
		MSG_ERROR("'abspdf::integral1d(double a, double b, size_t dim, double eps)' should not be called for high dimensional pdf");
		return 0;
	}
	double nom = recursive_simpson(a, b, eps);
	double denom = recursive_simpson(min(0), max(0), eps);
	return nom/denom;
}

double abspdf::recursive_simpson(double a, double b, double eps)
{
	double c = (a+b)/2;
	double mid = simpson(a, b);
	double l = simpson(a,c);
	double r =simpson(c,b);
	if (fabs(l+r-mid) <= 15*eps) {
		return l+r+(l+r-mid)/15.0;
	}
	return recursive_simpson(a, c, eps/2.0) + recursive_simpson(c, b, eps/2.0);
}

double abspdf::simpson(double a, double b)
{
	double c = (a+b)/2;
	return (evaluate(&a) + evaluate(&b) + 4.0*evaluate(&c)) * (b-a)/6.0;
}

double abspdf::operator()(const double * x)
{
	return norm() * evaluate(x);
}

template<typename...T> void abspdf::plot_on(plot * frame, T...action)
{
	frame->add(this);
}

void abspdf::update_lastvalue()
{
	for (size_t u = 0; u < m_npara; ++u) {
		m_lastvalue[u] = m_paralist.at(u)->value();
	}
}

bool abspdf::updated()
{
	for (size_t u = 0; u < m_npara; ++u) {
		if (m_paralist.at(u)->value() != m_lastvalue.at(u)) {
			return true;
		}
	}
	return false;
}
