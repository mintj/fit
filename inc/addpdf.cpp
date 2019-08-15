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

void addpdf::draw_comp(TH1 * h, size_t n, TH1 * hnorm = 0, const char * option)
{
	if (m_dim && n < m_plist.size()) {
		pdf * p = m_plist[n];
		h->Reset();
		for (size_t u = 0; u < m_normset->size(); ++u) {
			double v = p->evaluate(m_normset->at(u)) * m_normset->weight(u);
			h->Fill(m_normset->at(u)[0], v);
		}
		if (hnorm) h->Scale(hnorm->Integral() / h->Integral());
		calculate_frac();
		h->Scale(m_frac[n]);
		for (int u = 1; u <= h->GetNbinsX(); ++u) {
			h->SetBinError(u, 0);
		}
		h->Draw(option);
	}
}

void addpdf::draw_comp(TH2 * h, size_t n, TH2 * hnorm = 0, const char * option)
{
	if (m_dim && n < m_plist.size()) {
		pdf * p = m_plist[n];
		h->Reset();
		for (size_t u = 0; u < m_normset->size(); ++u) {
			double v = p->evaluate(m_normset->at(u)) * m_normset->weight(u);
			h->Fill(m_normset->at(u)[0], m_normset->at(u)[1], v);
		}
		if (hnorm) h->Scale(hnorm->Integral() / h->Integral());
		calculate_frac();
		h->Scale(m_frac[n]);
		for (int u = 1; u <= h->GetNbinsX(); ++u) {
			for (int v = 1; v <= h->GetNbinsY(); ++v) {
				h->SetBinError(u, v, 0);
			}
		}
		h->Draw(option);
	}
	else {
		std::cout << "[pdf] error: 1d pdf cannot plot 2d hist" << std::endl;
	}
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
