#ifndef ADDPDF_H__
#define ADDPDF_H__

#include <vector>
#include "pdf.h"

class chi2fcn;
class datahist;
class dataset;
class variable;

class addpdf: public pdf
{
	public:
		addpdf(const std::vector<pdf *> plist, const std::vector<variable *> flist, dataset * normset = 0);
		virtual ~addpdf();
		void chi2fit(datahist * data, bool minos_err = false);
		chi2fcn * create_chi2(datahist * data);
		virtual double evaluate(const double * x);
		//virtual double integral(double a, double b, int n = 0);
		//virtual double log_sum(dataset * data);
		//virtual double nevt();
		virtual double norm() { return 1; }
		virtual bool normalized() { return true; }
		virtual void set_normset(dataset * normset);

	protected:
		void init();

	protected:
		std::vector<pdf *> m_plist;
		std::vector<variable *> m_flist;
		std::shared_ptr<chi2fcn> m_chi2;
};

#endif
