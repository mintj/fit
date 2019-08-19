#include "TString.h"
#include "dataset.h"
#include "projpdf.h"
#include "variable.h"
		
projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, double lo, double hi):
	pdf(1, vlist, normset),
	m_bin_weight(nbin, 0),
	m_pdim(pdim)
{
	assert(pdim < normset.dim() && nbin > 0);
	m_hist = new TH1F("unnamed", "", nbin, lo, hi);
	init();
}

projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, const double * binning):
	pdf(1, vlist, normset),
	m_bin_weight(nbin, 0),
	m_pdim(pdim)
{
	assert(pdim < normset.dim() && nbin > 0);
	m_hist = new TH1F("unnamed", "", nbin, binning);
	init();
}

projpdf::~projpdf()
{
}

double projpdf::evaluate(const double * x)
{
	normalize();
	int bin = m_hist->FindBin(x[0]);
	if (bin > 0 && bin <= m_hist->GetNbinsX()) {
		return m_hist->GetBinContent(bin);
	}
	return 0;
}

//int projpdf::find_bin(double x)
//{
//	for (size_t u = 0; u < m_binning.size()-1; ++u) {
//		if (x >= m_binning[u] && x < m_binning[u+1]) return u;
//	}
//	return -1;
//}

void projpdf::init()
{
	m_hist->SetName(Form("%p", m_hist));
	for (size_t u = 0; u < m_normset->size(); ++u) {
		int bin = m_hist->FindBin(m_normset->at(u)[m_pdim]);
		if (bin > 0 && bin <= m_hist->GetNbinsX()) {
			m_bin_weight[bin-1] += m_normset->weight(u);
		}
	}
}

void projpdf::project_to_hist()
{
	m_hist->Reset();
	for (size_t u = 0; u < m_normset->size(); ++u) {
		int bin = m_hist->FindBin(m_normset->at(u)[m_pdim]);
		if (bin > 0 && bin <= m_hist->GetNbinsX()) {
			m_hist->Fill(m_normset->at(u)[m_pdim], func_weight(m_normset->at(u)) * m_normset->weight(u));
		}
	}
	double tot = 0;
	for (int u = 1; u <= m_hist->GetNbinsX(); ++u) {
		double c = m_hist->GetBinContent(u);
		double w = m_bin_weight[u-1];
		//m_hist->SetBinContent(u, c/w);
		m_hist->SetBinContent(u, c);
		tot += c;
	}
	m_hist->Scale(1.0/tot);
}

double projpdf::norm()
{
	return 1;
}

int projpdf::normalize()
{
	if (updated()) {
		m_norm = 1;
		project_to_hist();
		update_lastvalue();
		return 0;
	}
	return m_status;
}
