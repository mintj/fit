#include "dataset.h"
#include "projpdf.h"
#include "variable.h"
		
projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t projdim, size_t nbin, double lo, double hi):
	pdf(1, vlist, normset)
{
	assert(projdim < normset.dim() && nbin > 0);
	double xlo = (lo < hi) ? lo : hi;
	double xhi = (lo < hi) ? hi : lo;
	double step = (xhi-xlo)/nbin;
	for (size_t u = 0; u <= nbin; ++u) {
		m_binning.push_back(xlo + u*step);
		m_bin_data.push_back(std::vector<double>(0));
		m_bin_weight.push_back(std::vector<double>(0));
	}
	init(projdim);
}

projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t projdim, size_t nbin, const double * binning):
	pdf(1, vlist, normset)
{
	assert(projdim < normset.dim() && nbin > 0);
	for (size_t u = 0; u < nbin; ++u) {
		m_binning.push_back(binning[u]);
		if (u > 0) {
			m_bin_data.push_back(std::vector<double>(0));
			m_bin_weight.push_back(std::vector<double>(0));
		}
	}
	init(projdim);
}

projpdf::~projpdf()
{
}

double projpdf::evaluate(const double * x)
{
	double v = 0;
	int bin = find_bin(x[0]);
	if (bin >= 0) {
		for (size_t u = 0; u < m_bin_data[bin].size(); ++u) {
			double xx = m_bin_data[bin][u];
			double ww = m_bin_weight[bin][u];
			v += func_weight(xx) * ww;
		}
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

void projpdf::init(size_t projdim)
{
	for (size_t u = 0; u < m_normset->size(); ++u) {
		double x = m_normset->at(u)[projdim];
		int bin = find_bin(x);
		if (bin >= 0) {
			m_bin_data[bin].push_back(x);
			m_bin_weight[bin].push_back(m_normset->weight(u));
		}
	}
}
