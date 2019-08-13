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
