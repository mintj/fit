#include "dataset.h"
#include "projpdf.h"
#include "variable.h"
		
projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, double lo, double hi):
	pdf(1, vlist, normset)
{
	assert(pdim < normset.dim() && nbin > 0);
	double xlo = (lo < hi) ? lo : hi;
	double xhi = (lo < hi) ? hi : lo;
	double step = (xhi-xlo)/nbin;
	for (size_t u = 0; u <= nbin; ++u) {
		m_binning.push_back(xlo + u*step);
		if (u > 0) {
			m_bin_data.push_back(std::vector<double *>(0));
			m_bin_weight.push_back(std::vector<double>(0));
		}
	}
	init(pdim);
}

projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, const double * binning):
	pdf(1, vlist, normset)
{
	assert(pdim < normset.dim() && nbin > 0);
	for (size_t u = 0; u <= nbin; ++u) {
		m_binning.push_back(binning[u]);
		if (u > 0) {
			m_bin_data.push_back(std::vector<double *>(0));
			m_bin_weight.push_back(std::vector<double>(0));
		}
	}
	init(pdim);
}

projpdf::~projpdf()
{
}

void projpdf::draw(TH1 * h, TH1 * hnorm, const char * option)
{
	const char * name = h->GetName();
	if (h->IsOnHeap()) delete h;
	h = new TH1F(name, "", m_binning.size()-1, &m_binning[0]);
	pdf::draw(h, hnorm, option);
}

double projpdf::evaluate(const double * x)
{
	double v = 0;
	int bin = find_bin(x[0]);
	if (bin >= 0) {
		for (size_t u = 0; u < m_bin_data[bin].size(); ++u) {
			double * xx = m_bin_data[bin][u];
			double w = m_bin_weight[bin][u];
			v += func_weight(xx) * w;
		}
		v /= (m_binning[bin+1] - m_binning[bin]);
	}
	return v;
}

int projpdf::find_bin(double x)
{
	for (size_t u = 0; u < m_binning.size()-1; ++u) {
		if (x >= m_binning[u] && x < m_binning[u+1]) return u;
	}
	return -1;
}

void projpdf::init(size_t pdim)
{
	for (size_t u = 0; u < m_normset->size(); ++u) {
		double x = m_normset->at(u)[pdim];
		int bin = find_bin(x);
		if (bin >= 0) {
			m_bin_data[bin].push_back(m_normset->at(u));
			m_bin_weight[bin].push_back(m_normset->weight(u));
		}
	}
}
