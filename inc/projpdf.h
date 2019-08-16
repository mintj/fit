#ifndef PROJPDF_H__
#define PROJPDF_H__

#include <vector>
#include "pdf.h"

class variable;
class dataset;

class projpdf: public pdf
{
	public:
		projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, double lo, double hi);
		projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, const double * binning);
		virtual ~projpdf();

		const std::vector<double> & get_binning() { return m_binning; }
		size_t proj_dim() { return m_pdim; }
		
		// override pdf
		double evaluate(const double * x);
		double norm();
		
		virtual double func_weight(const double * x) = 0;

	protected:
		int find_bin(double x);
		void init(size_t pdim);

	protected:
		size_t m_pdim;
		std::vector<double> m_binning;
		std::vector<double> m_bin_totweight;
		std::vector<std::vector<double *>> m_bin_data;
		std::vector<std::vector<double>> m_bin_weight;
};

#endif
