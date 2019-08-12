#ifndef EXTPDF_H__
#define EXTPDF_H__

#include <vector>
#include "pdf.h"

class chi2fcn;
class datahist;
class dataset;
class variable;

class extpdf: public pdf
{
	public:
		extpdf(pdf * p, variable * n, dataset * normset = 0);
		extpdf(const std::vector<pdf *> plist, const std::vector<variable *> nlist, dataset * normset = 0);
		virtual ~extpdf();
		void chi2fit(datahist * data, bool minos_err = false);
		chi2fcn * create_chi2(datahist * data);
		virtual double evaluate(const double * x);
		virtual double integral(double a, double b, int n = 0);
		virtual double log_sum(dataset * data);
		virtual double nevt();
		virtual double norm() { return 1; }
		virtual void set_normset(dataset * normset);

	protected:
		void init();

	protected:
		std::vector<pdf *> m_plist;
		std::vector<variable *> m_nlist;
		std::shared_ptr<chi2fcn> m_chi2;
};

#endif
