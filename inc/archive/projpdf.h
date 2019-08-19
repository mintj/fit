#ifndef PROJPDF_H__
#define PROJPDF_H__

#include <vector>
#include "TH1F.h"
#include "pdf.h"

class variable;
class dataset;

class projpdf: public pdf
{
	public:
		projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, double lo, double hi);
		projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, const double * binning);
		virtual ~projpdf();

		TH1F * hist() { return m_hist; }
		size_t proj_dim() { return m_pdim; }
		
		// override pdf
		double evaluate(const double * x);
		double norm();
		int normalize();
		
		virtual double func_weight(const double * x) = 0;

	protected:
		void init();
		void project_to_hist();

	protected:
		size_t m_pdim;
		std::vector<double> m_bin_weight;
		TH1F * m_hist;
};

#endif
