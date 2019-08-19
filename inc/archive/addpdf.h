#ifndef ADDPDF_H__
#define ADDPDF_H__

#include <vector>
#include "pdf.h"

class dataset;
class variable;

class addpdf: public pdf
{
	public:
		addpdf(const std::vector<pdf *> plist, const std::vector<variable *> flist);
		virtual ~addpdf();
		
		void calculate_frac();
		double frac(size_t n);
		size_t number_of_pdfs() { return m_plist.size(); }
		void set_frac_for_plot(size_t n, double f);
		
		// override pdf
		double evaluate(const double * x);
		double evaluate_for_plot(const double * x);
		double integral(double a, double b, int n = 0);
		double norm() { return 1; }
		bool normalized() { return true; }
		void set_normset(dataset & normset);

	protected:
		void init();

	protected:
		std::vector<double> m_frac;
		std::vector<pdf *> m_plist;
		std::vector<variable *> m_flist;
};

#endif
