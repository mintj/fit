#ifndef GAUS_PDF_H__
#define GAUS_PDF_H__

#include "data.h"
#include "pdf_base.h"

class gaus_pdf: public pdf_base
{
	public:
		gaus_pdf(variable & x, variable & mean, variable & sigma, data & data_norm);
		virtual ~gaus_pdf();
		virtual double evaluate(const std::vector<double> & point) const;
};

#endif
