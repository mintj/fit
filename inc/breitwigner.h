#ifndef BREITWIGNER1D_H__
#define BREITWIGNER1D_H__

#include "pdf.h"

class dataset;
class variable;

class breitwigner: public pdf
{
	public:
		breitwigner(variable & m, variable & s, dataset * normset = 0);
		virtual ~breitwigner();
		virtual double evaluate(const double * x);
};

#endif
