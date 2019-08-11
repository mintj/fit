#include <iostream>
#include "TMath.h"
#include "dataset.h"
#include "extpdf.h"
#include "variable.h"

extpdf::extpdf(pdf * p, variable * nevt, dataset * normset):
	pdf(),
	m_plist(1, p),
	m_nlist(1, nevt)
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

double extpdf::evaluate(const double * x)
{
	double v = 0;
	double ntot = 0;
	for (size_t u = 0; u < m_plist.size(); ++u) {
		double raw_value = m_plist.at(u)->evaluate(x);
		double norm_factor = m_plist.at(u)->norm();
		double nevt = m_nlist.at(u)->value();
		//std::cout << "(raw, norm, nevt) = (" << raw_value << ", " << norm_factor << ", " << nevt << ")" << std::endl;
		v += nevt * norm_factor * raw_value;
		ntot += nevt;
	}
	return v/ntot;
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
	//for (size_t u = 0; u < m_plist.size(); ++u) {
	//	pdf * curr_pdf = m_plist.at(u);
	//	variable * curr_nevt = m_nlist.at(u);
	//	curr_pdf->set_normset(m_normset);
	//	m_dim = curr_pdf->dim();

	//	for (size_t vid = 0; vid < curr_pdf->npar_ext(); ++vid) {
	//		variable * v = curr_pdf->get_var_ext(vid);
	//		m_var_ext.push_back(v);
	//	}
	//	m_var_ext.push_back(curr_nevt);

	//	for (size_t vid = 0; vid < curr_pdf->npar_int(); ++vid) {
	//		variable * v = curr_pdf->get_var_int(vid);
	//		if (m_vcount.find(v) == m_vcount.end()) {
	//			m_var_int.push_back(v);
	//		}
	//		++m_vcount[v];
	//	}
	//	if (m_vcount.find(curr_nevt) == m_vcount.end()) {
	//		m_var_int.push_back(curr_nevt);
	//	}
	//	++m_vcount[curr_nevt];
	//}
}

double extpdf::log_sum(dataset * data)
{
	double nevt = 0;
	for (size_t u = 0; u < m_nlist.size(); ++u) {
		nevt += m_nlist.at(u)->value();
	}
	//std::cout << "nevt = " << nevt << ", ndata = " << data->size() << std::endl;
	return pdf::log_sum(data) + log(TMath::Poisson(nevt, data->size()));
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
