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
		void draw_comp(TH1 * h, size_t n, TH1 * hnorm = 0, const char * option = "hist same");
		void draw_comp(TH2 * h, size_t n, TH2 * hnorm = 0, const char * option = "hist same");
		
		// override pdf
		virtual double evaluate(const double * x);
		virtual double integral(double a, double b, int n = 0);
		virtual double norm() { return 1; }
		virtual bool normalized() { return true; }
		virtual void set_normset(dataset & normset);

	protected:
		void init();

	protected:
		std::vector<double> m_frac;
		std::vector<pdf *> m_plist;
		std::vector<variable *> m_flist;
};

#endif
