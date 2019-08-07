#ifndef GAUSS1D_H__
#define GAUSS1D_H__

#include "pdf.h"

class dataset;
class variable;

class gauss1d: public pdf
{
	public:
		gauss1d(variable & m, variable & s, dataset * normset);
		virtual ~gauss1d();
		virtual double evaluate(const double * x, const std::vector<double> & par);
};

#endif
