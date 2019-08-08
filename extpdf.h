#ifndef EXTPDF_H__
#define EXTPDF_H__

#include <vector>
#include "pdf.h"

class dataset;
class variable;

class extpdf: public pdf
{
	public:
		extpdf(pdf * p, variable * nevt, dataset * normset = 0);
		extpdf(const std::vector<pdf *> plist, const std::vector<variable *> nlist, dataset * normset = 0);
		virtual ~extpdf();
		virtual double evaluate(const double * x);
		virtual double log_sum(dataset * data);
		virtual double norm() { return 1; }
		virtual void set_normset(dataset * normset);

	protected:
		void init();

	protected:
		std::vector<pdf *> m_plist;
		std::vector<variable *> m_nlist;
};

#endif
