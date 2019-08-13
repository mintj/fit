#ifndef GAUSS1D_H__
#define GAUSS1D_H__

#include "pdf.h"

class dataset;
class variable;

class gaussian: public pdf
{
	public:
		gaussian(variable & m, variable & s, dataset & normset);
		virtual ~gaussian();
		virtual double evaluate(const double * x);
};

#endif
